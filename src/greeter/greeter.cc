#include <filesystem>
#include <fstream>

#include <gtkmm.h>

#include "greeter/greeter.hh"
#include "greeter/window.hh"
#include "widgets/profile.hh"
#include "utils.hh"

using better::Greeter;


auto
Greeter::get_greeter_file( std::string_view p_name ) -> std::string
{
    namespace fs = std::filesystem;

    static constexpr std::array<std::string_view, 3> BASE_PATHS {{
        "/org/BetterDE/BetterGreeter",
        "/etc/greetd/better",
        "."
    }};

    for (const std::string_view &base : BASE_PATHS) {
        fs::path path { fs::path(base) / p_name };

        if (fs::exists(path)) return path.string();
    }

    g_critical("Program file %s does not exist.", p_name.data());
    return "";
}


auto
Greeter::get_builder( void ) -> Glib::RefPtr<Gtk::Builder> &
{
    using Builder = Gtk::Builder;
    static Glib::RefPtr<Builder> builder;

    if (!builder)
        builder = Builder::create_from_file(get_greeter_file("greeter.xml"));
    return builder;
}


auto
Greeter::get_users() -> std::vector<std::pair<std::string, std::string>>
{
    std::ifstream passwd { std::filesystem::path(PASSWD_PATH) };

    std::vector<std::pair<std::string, std::string>> users;
    users.reserve(2);

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
        if (utils::to_int(buffer[2]) < 1000) continue;
        if (buffer[0] == "nobody") continue;

        users.emplace_back(buffer[0], buffer[5]);
    }

    return users;
}


auto
Greeter::get_profile_picture( const str_pair &p_user ) -> Profile *
{
    namespace fs = std::filesystem;
    std::string path;

    do { /* $HOME/.face */
        fs::path pfp_path { fs::path(p_user.second) / ".face" };
        if (!fs::exists(pfp_path) || !fs::is_regular_file(pfp_path))
            break;

        path = pfp_path.string();
    } while (0);

    if (path.empty()) path = "/usr/share/pixmaps/faces/default.png";

    return Gtk::make_managed<Profile>(path);
}


Greeter::Greeter( void ) :
    m_app(Gtk::Application::create(APP_ID))
{
    load_css();
}


auto
Greeter::run( void ) -> int
{ return m_app->make_window_and_run<better::GreeterWindow>(0, nullptr); }


void
Greeter::load_css( void )
{
    auto provider { Gtk::CssProvider::create() };
    provider->load_from_path(get_greeter_file("style.css"));

    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(),
        provider,
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
}