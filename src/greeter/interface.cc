#include <chrono>

#include <json/value.h>
#include <glibmm.h>

#include "greeter/interface.hh"
#include "greeter/utils.hh"
#include "greeter/app.hh"
#include "greeter/log.hh"

using greeter::Interface;


Interface::Interface( request_sig &p_request, respond_sig &p_respond ) :
    m_container(Gtk::Orientation::VERTICAL),
    m_users(greeter::get_users()),
    m_request(p_request),
    m_respond(p_respond)
{
    auto users { greeter::get_users() };
    if (m_users.empty()) [[unlikely]]
    {
        log::write<ERROR>("Failed to open /etc/passwd for reading.");
        std::exit(1);
    }

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

        m_accessibility->set_has_frame(false);
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
    }

    {
        auto *user_container { Gtk::make_managed<Gtk::Box>() };
        user_container->set_orientation(Gtk::Orientation::VERTICAL);
        user_container->set_vexpand(true);
        user_container->set_halign(Gtk::Align::CENTER);
        user_container->set_valign(Gtk::Align::CENTER);

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
        m_pfp->set_halign(Gtk::Align::CENTER);
        m_pfp->set_valign(Gtk::Align::CENTER);
        m_pfp->set_name("better-greeter-pfp");
        user_container->append(*m_pfp);

        auto *username_container { Gtk::make_managed<Gtk::Box>() };

        m_username->set_hexpand(true);
        m_username->set_halign(Gtk::Align::CENTER);
        m_username->set_label(last_picked_user.asString());
        m_username->set_name("better-greeter-username");

        m_username_switcher->set_has_frame(false);
        m_username_switcher->set_icon_name("media-playback-start-symbolic");
        m_username_switcher->set_halign(Gtk::Align::START);
        m_username_switcher->set_name("better-greeter-username-switcher");

        if (m_users.size() == 1)
            m_username_switcher->set_visible(false);

        username_container->append(*m_username);
        username_container->append(*m_username_switcher);
        username_container->set_margin_top(10);
        username_container->set_halign(Gtk::Align::CENTER);
        username_container->set_valign(Gtk::Align::START);
        user_container->append(*username_container);

        m_container->append(*user_container);

        m_username_switcher->signal_clicked().connect(sigc::mem_fun(
            *this, &Interface::on_username_switch));
    }

    {

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