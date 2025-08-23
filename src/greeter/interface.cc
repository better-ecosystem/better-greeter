#include <chrono>
#include <json/value.h>
#include <glibmm.h>
#include "greeter/interface.hh"
#include "greeter/app.hh"

using greeter::Interface;


Interface::Interface( void ) :
    m_container(Gtk::Orientation::VERTICAL),
    m_clock("00:00"),
    m_pfp(),
    m_username()
{
    this->fullscreen();
    this->set_child(*m_container);

    this->create_widgets();

    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &Interface::update_clock), 60);
}


void
Interface::create_widgets( void )
{
    m_container->set_name("better-greeter-container");

    { /* Create top widgets */
        auto *top_container { Gtk::make_managed<Gtk::Box>() };

        m_clock->set_justify(Gtk::Justification::CENTER);
        m_clock->set_hexpand(true);
        m_clock->set_halign(Gtk::Align::CENTER);
        m_clock->set_name("better-greeter-clock");

        m_accessibility->set_halign(Gtk::Align::END);
        m_accessibility->set_name("better-greeter-accessability");
        m_accessibility->set_icon_name(
            "preferences-desktop-accessibility-symbolic");

        top_container->set_valign(Gtk::Align::START);
        top_container->set_halign(Gtk::Align::FILL);
        top_container->set_name("better-greeter-top-container");
        top_container->append(*m_clock);
        top_container->append(*m_accessibility);

        m_container->append(*top_container);
        m_container->set_vexpand(true);
        m_container->set_valign(Gtk::Align::START);
    }

    {
        auto users { App::get_users() };
        auto last_picked_user { App::get_cache()["username"].asString() };

        if (App::set_pfp(*m_pfp, users[last_picked_user])) [[likely]]
        { /* If the user have a profile picture */
            m_pfp->set_size_request(64, 64);
            m_pfp->set_halign(Gtk::Align::CENTER);
            m_pfp->set_valign(Gtk::Align::CENTER);
            m_pfp->set_name("better-greeter-pfp");
            m_container->append(*m_pfp);
        }

        m_username->set_vexpand(true);
        m_username->set_halign(Gtk::Align::CENTER);
        m_username->set_valign(Gtk::Align::CENTER);
        m_username->set_label(last_picked_user);
        m_username->set_name("better-greeter-username");
        m_container->append(*m_username);
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