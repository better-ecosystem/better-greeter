#pragma once
#include <filesystem>
#include <gtkmm/picture.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include "greeter/ipc.hh"

namespace Gtk { class Builder; }


namespace greeter
{
    namespace fs = std::filesystem;


    /**
     * A class that manages the user interface of better-greeter.
     */
    class Interface : public Gtk::Window
    {
    public:
        Interface( const sigc::signal<Socket::Response (
                                const Socket::Request & )> &p_signal );


    private:
        Gtk::Label  *m_clock;
        Gtk::Button *m_accessibility;

        Gtk::Picture *m_pfp;
        Gtk::Label   *m_username;
        Gtk::Button  *m_username_switcher;

        Gtk::Entry *m_password;

        Gtk::Button *m_settings;

        std::map<std::string, fs::path> m_users;

        sigc::signal<Socket::Response ( const Socket::Request & )> m_signal;

    protected:
        void setup_widgets( const std::shared_ptr<Gtk::Builder> &p_builder );


        /**
         * @brief Timeout function to update @e m_clock every second.
         */
        auto update_clock( void ) -> bool;


        /**
         * @brief A function that is triggered when
         *        the user press on @e m_username_switcher .
         */
        void on_username_switch( void );
    };
}