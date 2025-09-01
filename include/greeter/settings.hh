#pragma once
#include <vector>

namespace Gtk
{
    class Window;
}


namespace greeter
{
    struct Session;


    class Settings
    {
    public:
        Settings( void );


        [[nodiscard]]
        auto get_selected_session( void ) const -> Session;


        void open_window( void );

    private:
        std::vector<Session> m_sessions;

        Gtk::Window *m_window;
    };
}