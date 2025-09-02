#include <cstdlib>
#include <fstream>

#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <giomm.h>

#include "greeter/ipc.hh"
#include "greeter/log.hh"

using greeter::Socket;


Socket::Socket( void )
{
    static const char *GREETD_SOCKET { std::getenv("GREETD_SOCKET") };
    if (GREETD_SOCKET == nullptr) {
        greeter::log::write<LogLevel::ERROR>("GREETD_SOCKET not found.");
        std::exit(1);
    }

    m_client = Gio::SocketClient::create();
    auto connectable { Gio::UnixSocketAddress::create(GREETD_SOCKET) };
    m_connection = m_client->connect(connectable);

    m_ostream = m_connection->get_output_stream();
    m_istream = m_connection->get_input_stream();
}


auto
Socket::write( const Request &p_msg ) -> Response
{
    Json::Value msg;

    switch (p_msg.type) {
        case CREATE_SESSION:
            msg["type"] = "create_session";
            msg["username"] = p_msg.msg;
            break;
        case POST_AUTH_RESPONSE:
            msg["type"] = "post_auth_message_request";
            msg["response"] = p_msg.msg;
            break;
        case START_SESSION:
            msg["type"] = "start_session";
            msg["cmd"]  = Json::Value { Json::arrayValue };
            msg["env"]  = Json::Value { Json::arrayValue };

            for (const std::string &arg : p_msg.session.args)
                msg["cmd"].append(arg);
            for (const std::string &env : p_msg.session.envs)
                msg["env"].append(env);

            break;
        case CANCEL_SESSION:
            msg["type"] = "cancel_session";
            break;
    }

    send_message(msg);

    return get_message();
}


auto
Socket::request_auth( const std::string &p_username,
                      const std::string &p_password,
                      const Session     &p_session ) -> Response
{
    Request create_session { CREATE_SESSION, p_username };
    Response msg { write(create_session) };

    std::ofstream out { "/home/kei/project/better/greeter/out.txt" };
    out << msg.msg_type << ' ' << msg.msg_desc << ' ' << static_cast<uint32_t>(msg.type);
}


void
Socket::send_message( const Json::Value &p_msg )
{
    uint32_t len { p_msg.size() };
    uint32_t len_net { g_htonl(len) };
    std::array<int8_t, 4> len_buff;
    std::memcpy(len_buff.data(), &len_net, 4);

    m_ostream->write(len_buff.data(), len_buff.size());
    m_ostream->write(p_msg.asString());
    m_ostream->flush();
}


auto
Socket::get_message( void ) -> Response
{
    uint32_t len { 0 };
    m_istream->read(&len, 4);

    if (len == 0) {
        /* TODO: Error handling here. */
        return { ERROR, "", "" };
    }

    std::string buff;
    buff.reserve(len);
    m_istream->read(buff.data(), len);

    Response response;
    Json::Value msg;
    std::istringstream { buff } >> msg;

    std::string type { msg["type"].asString() };
    if (type == "success") {
        response.type = SUCCESS;
        return response;
    } else if (type == "error") {
        response.type = ERROR;
        response.msg_type = msg["error_type"].asString();
        response.msg_desc = msg["description"].asString();
    }

    response.type = MESSAGE;
    response.msg_type = msg["auth_message_type"].asString();
    response.msg_desc = msg["auth_message"].asString();
    return response;
}