#include <gtkmm.h>
#include "greeter/greeter.hh"
#include "greeter/window.hh"

using better::GreeterWindow;


GreeterWindow::GreeterWindow( void )
{
    this->set_title("Better Greeter");
    this->set_name("greeter-window");

    this->fullscreen();

    auto builder { better::Greeter::get_builder() };
    this->set_child(*builder->get_widget<Gtk::Box>("greeter-container"));
}


GreeterWindow::GreeterWindow( BaseObjectType                   *,
                              const Glib::RefPtr<Gtk::Builder> & )
{}