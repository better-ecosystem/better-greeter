#include <fstream>
#include <print>

#include <json/json.h>
#include <gtkmm.h>

#include "greeter/interface.hh"
#include "greeter/app.hh"

using greeter::App;


auto
App::get_app_file( std::string_view p_path ) -> std::string
{
    for (const std::string_view &base : BASE_PATHS) {
        fs::path path { fs::path(base) / p_path };
        if (fs::exists(path)) return path.string();
    }

    return "";
}


auto
App::get_users( void ) -> std::map<std::string, fs::path>
{
    std::map<std::string, fs::path> users;

    std::ifstream passwd { "/etc/passwd" };
    if (!passwd.is_open()) {
        std::println("ERROR: Failed opening /etc/passwd: {}",
                      std::strerror(errno));
        /* TODO: Make a better error handler. */
        std::exit(1);
    }

    std::array<std::string, 7> buffer;

    for (std::string line; std::getline(passwd, line);) {
        {
            std::istringstream iss { line };

            size_t i { 0 };
            for (std::string val; std::getline(iss, val, ':') && i < 7; i++)
                buffer[i] = val;
        }

        /* The parsed string will contain these values: */
        /* [0] = name
           [1] = "x"
           [2] = UID
           [3] = GID
           [4] = description
           [5] = HOME
           [6] = shell
        */
        int32_t res { 0 };
        auto [ ptr, ec ] { std::from_chars(buffer[2].data(),
                                           buffer[2].data() + buffer[2].size(),
                                           res) };
        if (ec != std::errc()) continue;
        if (res < 1000) continue;

        if (buffer[0] == "nobody") continue;

        users.emplace(buffer[0], buffer[5]);
    }

    return users;
}


auto
App::set_pfp( Gtk::Picture &p_pfp, const std::string &p_home ) -> bool
{
    auto is_valid_face { []( const fs::path &p_path ) -> bool {
        return fs::exists(p_path) && fs::is_regular_file(p_path);
    } };


    fs::path face1 { fs::path(p_home) / ".face" };
    fs::path face2 { fs::path(p_home) / ".face.icon" };

    if (is_valid_face(face1)) {
        p_pfp.set_filename(face1.string());
        return true;
    }

    if (is_valid_face(face2)) {
        p_pfp.set_filename(face2.string());
        return true;
    }

    return false;
}


auto
App::get_cache( void ) -> Json::Value
{
    std::ifstream cache_file { fs::path(CACHE_PATH) };
    if (!cache_file.is_open()) {
        std::println("ERROR: Failed opening {}: {}",
                      CACHE_PATH, std::strerror(errno));
        return Json::nullValue;
    }

    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;

    Json::Value root;
    std::string err;
    if (!Json::parseFromStream(builder, cache_file, &root, &err)) {
        std::println("ERROR: Failed to parse JSON: {}", err);
        return Json::nullValue;
    }

    return root;
}


void
App::write_cache( const Json::Value &p_val )
{
    std::ofstream cache_file { fs::path(CACHE_PATH) };
    if (!cache_file.is_open()) {
        std::println("ERROR: Failed opening {}: {}",
                      CACHE_PATH, std::strerror(errno));

        /* TODO: Make a better error handler. */
        std::exit(1);
    }

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "    ";
    cache_file << Json::writeString(writer, p_val);
}



/* Non static member definition */



App::App( void ) :
    m_app(Gtk::Application::create(APP_ID))
{
    if (!fs::exists(BASE_PATHS[0]))
        fs::create_directories(BASE_PATHS[0]);

    if (!fs::exists(CACHE_PATH)) {
        Json::Value cache;
        cache["username"] = Json::Value { App::get_users().begin()->first };
        write_cache(cache);
    }

    load_css();
}


auto
App::run( int p_argc, char **p_argv ) -> int
{ return m_app->make_window_and_run<greeter::Interface>(p_argc, p_argv); }


void
App::load_css( void )
{
    auto provider { Gtk::CssProvider::create() };
    provider->load_from_path(get_app_file("style.css"));

    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(),
        provider,
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
}