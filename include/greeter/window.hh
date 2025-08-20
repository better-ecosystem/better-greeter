#pragma once
#include <filesystem>
#include <gtkmm/window.h>

namespace fs = std::filesystem;


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
    class Profile;


    struct GreeterData
    {
        struct
        {
            Gtk::Label *clock;

            Profile *pfp;

            Gtk::Label *username;
            Gtk::Button *switch_user;

            Gtk::Entry *password;

            Gtk::DropDown *session;
        } widgets;

        std::vector<std::pair<std::string, fs::path>> users;
    };


    class GreeterWindow : public Gtk::Window
    {
    public:
        GreeterWindow( void );


    private:
        GreeterData m_data;
        size_t m_user_idx;


        void init_data( void );

        auto update_clock( void ) -> bool;
        void on_switch_user( void );
    };
}