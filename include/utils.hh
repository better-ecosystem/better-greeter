#pragma once
#include <string_view>
#include <cstdint>


namespace utils
{
    [[nodiscard]]
    auto to_int( std::string_view p_str ) -> int32_t;


    [[nodiscard]]
    auto getenv( const std::string &p_env ) -> std::string;
}