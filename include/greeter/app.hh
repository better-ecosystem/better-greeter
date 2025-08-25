#pragma once
#include <memory>
#include <gtkmm/application.h>

namespace Json { class Value; }
namespace Gtk  { class Picture; }


namespace greeter
{
    class App
    {
        using respond_sig = sigc::signal<void (
                            const std::pair<int32_t, std::string> & )>;
        using request_sig = sigc::signal<std::pair<int32_t, std::string> ()>;

    public:
        App( void );


        [[nodiscard]]
        auto run( int p_argc, char **p_argv ) -> int;


    private:
        std::shared_ptr<Gtk::Application> m_app;

        request_sig m_req_signal;
        respond_sig m_res_signal;


        void load_css( void );
    };
}