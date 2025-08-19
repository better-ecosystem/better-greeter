#pragma once
#include <gtkmm/window.h>

namespace Gtk
{
    class DropDown;
    class Builder;
    class Button;
    class Entry;
    class Label;
    class Box;
}


namespace better
{
    struct GreeterWidgets
    {
        Gtk::Label *clock;
        Gtk::Box *pfp_container;

        Gtk::DropDown *username;
        Gtk::Entry *password;

        Gtk::DropDown *session;

        Gtk::Button *login;
    };


    class GreeterWindow : public Gtk::Window
    {
    public:
        GreeterWindow( void );


        GreeterWindow( BaseObjectType                   *p_cobject,
                       const Glib::RefPtr<Gtk::Builder> &p_builder );

    private:
        GreeterWidgets m_widgets;

        std::vector<std::pair<std::string, std::string>> m_users;


        auto update_clock( void ) -> bool;

        void update_pfp( void );
    };
}