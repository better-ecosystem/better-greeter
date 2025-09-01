#pragma once
#include <string>
#include "greeter/utils.hh"


namespace greeter
{
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