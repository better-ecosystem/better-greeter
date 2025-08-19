#pragma once
#include <memory>
#include <gtkmm/builder.h>


namespace Gtk
{
    class Application;
}
class Profile;


namespace better
{
    using str_pair = std::pair<std::string, std::string>;
    static constexpr std::string_view PASSWD_PATH { "/etc/passwd" };


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


        /**
         * Wrapper for `Gtk::Builder::get_widget` for getting widgets
         * from the greeter's internal ui file.
         */
        template<typename T_Widget>
        [[nodiscard]]
        static auto get_widget( const std::string &p_name ) -> T_Widget *
        { return get_builder()->get_widget<T_Widget>(p_name); }


        /**
         * Returns the list of `normal` users who have their own home directory.
         * Normal users are defined as users with ID >= 1000, and not nobody.
         */
        [[nodiscard]]
        static auto get_users( void ) -> std::vector<str_pair>;


        /**
         * Returns the user profile picture from $HOME/.face or
         * /usr/share/pixmaps/faces/default.png.
         */
        [[nodiscard]]
        static auto get_profile_picture( const str_pair &p_user ) -> Profile *;


        Greeter( void );


        auto run( void ) -> int;

    private:
        std::shared_ptr<Gtk::Application> m_app;


        void load_css( void );
    };
}
