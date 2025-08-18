#pragma once
#include <gtkmm/window.h>

namespace Gtk
{ class Builder; }


namespace better
{
    class GreeterWindow : public Gtk::Window
    {
    public:
        GreeterWindow( void );


        GreeterWindow( BaseObjectType                   *p_cobject,
                       const Glib::RefPtr<Gtk::Builder> &p_builder );

    private:

    };
}