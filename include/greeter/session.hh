#pragma once
#include <string>
#include <vector>
#include <array>


namespace greeter
{
    static constexpr std::array<std::string_view, 2> SESSION_PATHS {{
        "/usr/share/xsessions",
        "/usr/share/wayland-sessions"
    }};


    struct Session
    {
        bool X11;
        std::vector<std::string> envs;
        std::vector<std::string> args;


        Session( const std::string &p_bin );
        Session( void );

        /**
         * @brief Adds binary arguments to args[0], i.e. argv + 1, ...
         */
        void add_args( const std::vector<std::string> &p_args );


        /**
         * @brief Adds environment variables to the binary in the format
         *        KEY=VALUE.
         */
        void add_envs( const std::vector<std::string> &p_envs );


        /**
         * @brief Fetches "session" files from
         *        /usr/share/{xsessions,wayland-sessions}/
         *
         * @return A vector of Session.
         */
        [[nodiscard]]
        static auto get_sessions( void ) -> std::vector<Session>;
    };
}