#pragma once
#include <memory>
#include <gtkmm/builder.h>


namespace Gtk
{
    class Application;
}


namespace better
{
    class Greeter
    {
    public:
        /**
         * Returns a path pointing to the greeter's application file in
         * `/etc/greetd/better/@p p_name, or ./@p p_name `.
         */
        [[nodiscard]]
        static auto get_greeter_file( std::string_view p_name ) -> std::string;


        /**
         * Fetch a builder created from the greeter's builder file.
         * placed in `/org/BetterDE/BetterGreeter/greeter.xml`
         */
        [[nodiscard]]
        static auto get_builder( void ) -> std::shared_ptr<Gtk::Builder> &;


        template<typename T_Widget, typename... T_Args>
        [[nodiscard]]
        auto get_widget( const std::string &p_name,
                         T_Args        &&...p_args ) -> T_Widget *
        {
            return Gtk::Builder::get_widget_derived<T_Widget>(
                get_builder(), p_name, std::forward<T_Args>(p_args)...);
        }


        Greeter( void );


        auto run( void ) -> int;

    private:
        std::shared_ptr<Gtk::Application> m_app;


        void load_css( void );
    };
}
