#pragma once
#include <memory>
#include "greeter/session.hh"

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

            std::string msg;
            Session     session;

            Request( ReqType p_type, const std::string &p_msg = "" ) :
                type(p_type),
                msg(p_msg)
            {}


            Request( ReqType p_type, const Session &p_session ) :
                type(p_type),
                session(p_session)
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


        [[nodiscard]]
        auto write( const Request &p_msg ) -> Response;


        /**
         * @brief A wrapper for a bunch of @sa write functions.
         *
         * @param p_username The username to authenticate.
         * @param p_password The password to the user.
         * @param p_session  The session to start.
         * @return A Response struct, representing the first error found,
         *         or the last response after auth.
         */
        [[nodiscard]]
        auto request_auth( const std::string &p_username,
                           const std::string &p_password,
                           const Session     &p_session ) -> Response;

    private:
        std::shared_ptr<Gio::SocketClient>     m_client;
        std::shared_ptr<Gio::SocketConnection> m_connection;

        std::shared_ptr<Gio::OutputStream> m_ostream;
        std::shared_ptr<Gio::InputStream>  m_istream;


        void send_message( const Json::Value &p_msg );


        auto get_message( void ) -> Response;
    };
}