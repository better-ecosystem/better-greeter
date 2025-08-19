#include <charconv>
#include "utils.hh"


auto
utils::to_int( std::string_view p_str ) -> int32_t
{
    int32_t val { 0 };
    std::from_chars(p_str.data(), p_str.data() + p_str.size(), val);
    return val;
}