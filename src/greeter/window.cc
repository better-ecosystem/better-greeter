#include <chrono>

#include <gtkmm.h>

#include "greeter/greeter.hh"
#include "greeter/window.hh"

using better::GreeterWindow;


GreeterWindow::GreeterWindow( void ) :
    Gtk::Window(Greeter::get_widget<Gtk::Window>("greeter-window")->gobj())
{
    Glib::signal_timeout().connect(sigc::ptr_fun(&clock_updater), 1);
}


GreeterWindow::GreeterWindow( BaseObjectType                   *,
                              const Glib::RefPtr<Gtk::Builder> & )
{}


auto
GreeterWindow::clock_updater( void ) -> bool
{
    Greeter::get_widget<Gtk::Label>("greeter-clock")->set_label(
        std::format("{:%A %d %B %Y - %H:%M:%S}",
                     std::chrono::floor<std::chrono::seconds>(
                     std::chrono::system_clock::now())));
    return true;
}
