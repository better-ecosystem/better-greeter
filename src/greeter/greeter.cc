#include <filesystem>

#include <gtkmm.h>

#include "greeter/greeter.hh"
#include "greeter/window.hh"

using better::Greeter;


auto
Greeter::get_greeter_file( std::string_view p_name ) -> std::string
{
    namespace fs = std::filesystem;

    static constexpr std::array<std::string_view, 2> BASE_PATHS {{
        "/etc/greetd/better",
        "."
    }};

    for (const std::string_view &base : BASE_PATHS) {
        fs::path path { fs::path(base) / p_name };

        if (fs::exists(path)) return path.string();
    }
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