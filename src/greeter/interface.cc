#include <chrono>

#include <json/value.h>
#include <glibmm.h>
#include <gtkmm.h>

#include "greeter/interface.hh"
#include "greeter/utils.hh"
#include "greeter/app.hh"
#include "greeter/log.hh"

using greeter::Interface;


Interface::Interface( const sigc::signal<Socket::Response (
                      const Socket::Request & )> &p_signal ) :
    m_users(greeter::get_users()),
    m_signal(p_signal)
{
    auto users { greeter::get_users() };
    if (m_users.empty()) [[unlikely]]
    {
        log::write<ERROR>("Failed to open /etc/passwd for reading.");
        std::exit(1);
    }

    this->fullscreen();

    auto b { Gtk::Builder::create_from_file(
        greeter::get_app_file("greeter.xml")) };

    this->set_child(*b->get_widget<Gtk::Box>("better-greeter-container"));

    this->setup_widgets(b);

    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &Interface::update_clock), 60);
}


void
Interface::setup_widgets( const std::shared_ptr<Gtk::Builder> &p_b )
{
    m_pfp = p_b->get_widget<Gtk::Picture>("better-greeter-pfp");
    m_username = p_b->get_widget<Gtk::Label>("better-greeter-username");
    m_username_switcher = p_b->get_widget<Gtk::Button>(
        "better-greeter-username-switcher");
    m_password = p_b->get_widget<Gtk::Entry>("better-greeter-password");
    m_clock = p_b->get_widget<Gtk::Label>("better-greeter-clock");
    m_settings = p_b->get_widget<Gtk::Button>("better-greeter-settings");

    {
        Json::Value last_picked_user { greeter::get_cache() };

        /* TODO: Make a window showing up saying that a "thing" failed. */
        if (last_picked_user.isMember("err")) {
            log::write<ERROR>("Failed to parse cache file: {}",
                               last_picked_user["err"].asString());
            last_picked_user = Json::Value { m_users.begin()->first };
        } else last_picked_user = last_picked_user["username"];

        if (last_picked_user.isNull()) {
            log::write<ERROR>("Failed to open cache file for reading.");
            last_picked_user = Json::Value { m_users.begin()->first };
        }

        if (!greeter::set_pfp(*m_pfp, m_users[last_picked_user.asString()]))
            [[unlikely]] m_pfp->set_visible(false);
        else [[likely]] /* If the user have a profile picture */
            m_pfp->set_visible(true);

        m_username->set_label(last_picked_user.asString());
        if (m_users.size() == 1)
            m_username_switcher->set_visible(false);

        m_password->set_icon_from_icon_name(
            "view-reveal-symbolic", Gtk::Entry::IconPosition::SECONDARY);
        m_password->signal_icon_press().connect(
            [this]( Gtk::Entry::IconPosition p_pos )
            {
                if (p_pos != Gtk::Entry::IconPosition::SECONDARY) return;

                m_password->set_visibility(!m_password->get_visibility());
                m_password->set_icon_from_icon_name(
                    !m_password->get_visibility() ?
                    "view-reveal-symbolic" : "view-conceal-symbolic",
                    Gtk::Entry::IconPosition::SECONDARY
                );
            }
        );

        m_username_switcher->signal_clicked().connect(sigc::mem_fun(
            *this, &Interface::on_username_switch));

        
    }
}


auto
Interface::update_clock( void ) -> bool
{
    auto now { std::chrono::floor<std::chrono::seconds>(
               std::chrono::system_clock::now()) };

    m_clock->set_label(std::format("{:%H:%M}",
                       std::chrono::zoned_time {
                       std::chrono::current_zone(), now }));
    return true;
}


void
Interface::on_username_switch( void )
{ /* On username switch, we need to update `m_pfp`, and `m_username` */
    auto it { m_users.find(m_username->get_label()) };

    if (it == m_users.end() || std::next(it) == m_users.end())
        it = m_users.begin();
    else it++;

    m_username->set_label(it->first);
    if (greeter::set_pfp(*m_pfp, it->second)) [[likely]]
    { /* If the user have a profile picture */
        m_pfp->set_visible(true);
    } else [[unlikely]]
        m_pfp->set_visible(false);
}