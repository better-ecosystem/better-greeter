#include <chrono>

#include <gtkmm.h>

#include "greeter/greeter.hh"
#include "greeter/window.hh"
#include "widgets/profile.hh"

using better::GreeterWindow;


GreeterWindow::GreeterWindow( void ) :
    Gtk::Window(Greeter::get_widget<Gtk::Window>("greeter-window")->gobj()),
    m_users(Greeter::get_users())
{
    m_widgets = {
        .clock = Greeter::get_widget<Gtk::Label>("greeter-clock"),

        .pfp_container = Greeter::get_widget<Gtk::Box>("greeter-pfp-container"),
        .username = Greeter::get_widget<Gtk::DropDown>("greeter-username"),
        .password = {},

        .session = {},
        .login = {}
    };

    auto usernames { Gtk::StringList::create() };
    for (const auto &user : m_users)
        usernames->append(user.first);

    m_widgets.username->set_model(usernames);
    m_widgets.username->property_selected().signal_changed().connect(
        sigc::mem_fun(*this, &GreeterWindow::update_pfp));

    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &GreeterWindow::update_clock), 1);
}


GreeterWindow::GreeterWindow( BaseObjectType                   *,
                              const Glib::RefPtr<Gtk::Builder> & )
{}


auto
GreeterWindow::update_clock( void ) -> bool
{
    using std::chrono::system_clock;
    using std::chrono::seconds;

    m_widgets.clock->set_label(
        std::format("{:%A %d %B %Y - %H:%M:%S}",
                     std::chrono::floor<seconds>(system_clock::now())));
    return true;
}


void
GreeterWindow::update_pfp( void )
{
    guint idx { m_widgets.username->get_selected() };
    if (idx >= 0) {
        auto children { m_widgets.pfp_container->get_children() };
        for (auto child : children)
            m_widgets.pfp_container->remove(*child);

        Profile *pfp { Greeter::get_profile_picture(m_users.at(idx)) };
        m_widgets.pfp_container->append(*pfp);
    }
}
