#pragma once
#include <memory>
#include <gtkmm/application.h>
#include "greeter/ipc.hh"

namespace Json { class Value; }
namespace Gtk  { class Picture; }


namespace greeter
{
    /**
     * A class containing the base application and logic for better-greeter.
     */
    class App
    {
    public:
        App( void );


        [[nodiscard]]
        auto run( int p_argc, char **p_argv ) -> int;


    private:
        std::shared_ptr<Gtk::Application> m_app;

        Socket m_greetd_sock;

        sigc::signal<Socket::Response ( const Socket::Request & )> m_signal;

        void load_css( void );
    };
}