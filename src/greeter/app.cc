#include <iostream>

#include <json/json.h>
#include <gtkmm.h>

#include "greeter/interface.hh"
#include "greeter/utils.hh"
#include "greeter/app.hh"

using greeter::App;


App::App( void ) :
    m_app(Gtk::Application::create(APP_ID))
{
    std::string msg { greeter::check_and_create_app_dir() };
    if (!msg.empty() && msg != "created") {
        std::cerr << "Failed to create application file: " << msg << std::endl;
        std::exit(1);
    }

    load_css();
}


auto
App::run( int p_argc, char **p_argv ) -> int
{
    return m_app->make_window_and_run<greeter::Interface>(p_argc, p_argv,
           m_req_signal, m_res_signal);
}


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