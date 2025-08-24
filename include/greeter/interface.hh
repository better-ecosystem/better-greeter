#pragma once
#include <filesystem>
#include <gtkmm/picture.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include "widget_ptr.hh"


namespace greeter
{
    namespace fs = std::filesystem;


    class Interface : public Gtk::Window
    {
        using respond_sig = sigc::signal<void (
                            const std::pair<int32_t, std::string> & )>;
        using request_sig = sigc::signal<std::pair<int32_t, std::string> ()>;

    public:
        Interface( request_sig &p_request, respond_sig &p_respond );


    private:
        WidgetPtr<Gtk::Box> m_container;

        WidgetPtr<Gtk::Label> m_clock;
        WidgetPtr<Gtk::Button> m_accessibility;

        WidgetPtr<Gtk::Picture> m_pfp;
        WidgetPtr<Gtk::Label> m_username;
        WidgetPtr<Gtk::Button> m_username_switcher;

        WidgetPtr<Gtk::Entry> m_password;

        WidgetPtr<Gtk::Button> m_settings;

        std::map<std::string, fs::path> m_users;

        request_sig m_request;
        respond_sig m_respond;

    protected:
        void create_widgets( void );

        auto update_clock( void ) -> bool;

        void on_username_switch( void );
    };
}