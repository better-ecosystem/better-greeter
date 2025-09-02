#include <json/json.h>
#include <gtkmm.h>

#include "greeter/interface.hh"
#include "greeter/utils.hh"
#include "greeter/app.hh"
#include "greeter/log.hh"

using greeter::App;


App::App( void ) :
    m_app(Gtk::Application::create(APP_ID)),
    m_greetd_sock()
{
    std::string msg { utils::check_and_create_app_dir() };
    if (!msg.empty() && msg != "created") {
        log::write<ERROR>("Failed to create app file: {}", msg);
        std::exit(1);
    }

    if (msg.empty()) log::write<INFO>("Application file found.");

    load_css();
}


auto
App::run( int p_argc, char **p_argv ) -> int
{
    return m_app->make_window_and_run<greeter::Interface>(p_argc, p_argv,
           m_greetd_sock);
}


void
App::load_css( void )
{
    auto provider { Gtk::CssProvider::create() };
    provider->load_from_path(utils::get_app_file("style.css"));

    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(),
        provider,
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
}