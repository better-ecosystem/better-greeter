#pragma once
#include <filesystem>
#include <memory>
#include <gtkmm/application.h>

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


    class App
    {
    public:
        /**
         * Fetch an application file @p p_path from
         * @e /etc/greetd/better/ , or @e CWD
         */
        [[nodiscard]]
        static auto get_app_file( std::string_view p_path ) -> std::string;


        /**
         * Fetch normal @e users and their @e HOME from @e /etc/passwd
         * normal users being user with UID >= 1000
         */
        [[nodiscard]]
        static auto get_users( void ) -> std::unordered_map<std::string,
                                                            fs::path> &;


        /**
         * Sets @p p_pfp picture to @p p_home/.face or returns false.
         */
        [[nodiscard]]
        static auto set_pfp( Gtk::Picture      &p_pfp,
                             const std::string &p_home ) -> bool;


        /**
         * Gets the application's cache from @e /etc/greetd/better/cache.json
         */
        [[nodiscard]]
        static auto get_cache( void ) -> Json::Value;


        /**
         * Writes @p p_val to @e /etc/greetd/better/cache.json
         */
        static void write_cache( const Json::Value &p_val );


        App( void );


        [[nodiscard]]
        auto run( int p_argc, char **p_argv ) -> int;


    private:
        std::shared_ptr<Gtk::Application> m_app;


        void load_css( void );
    };
}