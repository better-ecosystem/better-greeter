#pragma once
#include <string>
#include <vector>


namespace greeter
{
    struct Session;


    class Settings
    {
    public:
        Settings( void );


        [[nodiscard]]
        auto get_selected_session( void ) const -> std::string;

    private:
        std::vector<Session> m_sessions;
    };
}