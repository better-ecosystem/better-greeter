#pragma once
#include <filesystem>
#include <gtkmm/picture.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include "widget_ptr.hh"


namespace greeter
{
    namespace fs = std::filesystem;


    class Interface : public Gtk::Window
    {
    public:
        Interface( void );


    private:
        WidgetPtr<Gtk::Box> m_container;

        WidgetPtr<Gtk::Label> m_clock;
        WidgetPtr<Gtk::Button> m_accessibility;

        WidgetPtr<Gtk::Picture> m_pfp;
        WidgetPtr<Gtk::Label> m_username;
        WidgetPtr<Gtk::Button> m_username_switcher;

        std::map<std::string, fs::path> m_users;

    protected:
        void create_widgets( void );

        auto update_clock( void ) -> bool;

        void on_username_switch( void );
    };
}