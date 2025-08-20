#include <chrono>

#include <gtkmm.h>

#include "greeter/greeter.hh"
#include "greeter/window.hh"
#include "widgets/profile.hh"

using better::GreeterWindow;
using better::Profile;


GreeterWindow::GreeterWindow( void ) :
    Gtk::Window(Greeter::get_widget<Gtk::Window>("greeter-window")->gobj()),
    m_user_idx(0)
{
    this->init_data();

    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &GreeterWindow::update_clock), 1);
}


void
GreeterWindow::init_data( void )
{
    m_data.users = Greeter::get_users();
    m_data.widgets = {
        .clock       = Greeter::get_widget<Gtk::Label>("greeter-clock"),
        .pfp         = nullptr,
        .username    = Greeter::get_widget<Gtk::Label>("greeter-username"),
        .switch_user = Greeter::get_widget<Gtk::Button>("greeter-switch-username"),
        .password    = nullptr,
        .session     = nullptr,
    };

    Profile *pfp {
        Gtk::Builder::get_widget_derived<Profile>(Greeter::get_builder(),
                                                 "greeter-pfp", 64) };
    m_data.widgets.pfp = pfp;
    m_data.widgets.pfp->set_visible(false);

    m_data.widgets.switch_user->signal_clicked().connect(
        sigc::mem_fun(*this, &GreeterWindow::on_switch_user));

    /* TODO: Add a cache that stores the last used user. */
    m_data.widgets.username->set_label(m_data.users[m_user_idx].first);
}


auto
GreeterWindow::update_clock( void ) -> bool
{
    using std::chrono::system_clock;
    using std::chrono::seconds;

    m_data.widgets.clock->set_label(
        std::format("{:%H:%M}",
                     std::chrono::floor<seconds>(system_clock::now())));
    return true;
}


void
GreeterWindow::on_switch_user( void )
{
    if ((m_user_idx + 1) == m_data.users.size())
        m_user_idx = 0;
    else m_user_idx++;

    usr_pair user { m_data.users.at(m_user_idx) };

    m_data.widgets.username->set_label(user.first);
    if (!user.second.empty())
        m_data.widgets.pfp->change_picture(user.second);
}