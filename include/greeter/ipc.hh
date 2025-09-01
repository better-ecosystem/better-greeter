#pragma once
#include <memory>
#include <vector>

namespace Gio
{
    class SocketConnection;
    class SocketClient;

    class OutputStream;
    class InputStream;
}

namespace Json { class Value; }


namespace greeter
{
    class Socket
    {
    public:
        enum ReqType
        {
            CREATE_SESSION,
            POST_AUTH_RESPONSE,
            START_SESSION,
            CANCEL_SESSION
        };


        struct Request
        {
            ReqType type;

            union
            {
                std::string msg;
                std::pair<std::vector<std::string>,
                          std::vector<std::string>> session;
            };

            Request( ReqType p_type, std::string p_msg = "" ) :
                type(p_type),
                msg(p_msg)
            {}


            Request( ReqType p_type, std::vector<std::string> p_cmd,
                                     std::vector<std::string> p_env ) :
                type(p_type),
                session({ std::move(p_cmd), std::move(p_env) })
            {}
        };

        enum ResponseType
        {
            SUCCESS,
            ERROR,
            MESSAGE,
        };

        struct Response
        {
            ResponseType type;

            std::string msg_type;
            std::string msg_desc;
        };


        Socket( void );


        auto write( const Request &p_msg ) -> Response;

    private:
        std::shared_ptr<Gio::SocketClient>     m_client;
        std::shared_ptr<Gio::SocketConnection> m_connection;

        std::shared_ptr<Gio::OutputStream> m_ostream;
        std::shared_ptr<Gio::InputStream>  m_istream;


        void send_message( const Json::Value &p_msg );


        auto get_message( void ) -> Response;
    };
}