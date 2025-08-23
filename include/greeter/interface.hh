#pragma once
#include <gtkmm/picture.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include "widget_ptr.hh"


namespace greeter
{

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

    protected:
        void create_widgets( void );

        auto update_clock( void ) -> bool;
    };
}