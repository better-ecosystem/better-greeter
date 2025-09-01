#pragma once
#include <string_view>
#include <filesystem>
#include <vector>
#include <array>
#include <map>

namespace Json { class Value; }
namespace Gtk  { class Picture; }


namespace greeter
{
    namespace fs = std::filesystem;

    static constexpr std::array<std::string_view, 3> BASE_PATHS {{
        "/etc/better/greeter",
        "."
    }};
    static constexpr std::string_view CACHE_PATH {
        "/etc/better/greeter/cache.json" };
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



    /**
     * @brief Checks whether the application directory and file exists,
     *        creates them if not.
     *
     * @return An empty string on found, "created" on successfully created,
     *         or the error message on errors.
     */
    [[nodiscard]]
    auto check_and_create_app_dir( void ) -> std::string;


    /**
     * @brief Fetch app file @p p_file from @e /etc/better/greeter or CWD.
     *
     * @return A valid path if the file is found, or an empty path if not.
     */
    [[nodiscard]]
    auto get_app_file( std::string_view p_file ) -> fs::path;


    /**
     * @brief Fetch users from @e /etc/passwd .
     *
     * @return A valid map of username and home directory,
     *         or an empty map on file failure.
     */
    [[nodiscard]]
    auto get_users( void ) -> std::map<std::string, fs::path>;


    /**
     * @brief Sets @p p_pfp picture to the @p p_home 's user pfp.
     *
     * @return true if the user have a pfp, false otherwise.
     */
    [[nodiscard]]
    auto set_pfp( Gtk::Picture      &p_pfp,
                  const std::string &p_home ) -> bool;


    /**
     * @brief Gets the application cache file from @e CACHE_PATH .
     *
     * @return A valid Json::Value on success,
     *         a Json::nullValue on file failure,
     *         or a Json::Value with "err" key set on parsing failure.
     */
    [[nodiscard]]
    auto get_cache( void ) -> Json::Value;


    /**
     * @brief Writes @p p_val to @e CACHE_PATH .
     *
     * @return true on success, or false on file failure.
     */
    [[nodiscard]]
    auto write_cache( const Json::Value &p_val ) -> bool;
}

