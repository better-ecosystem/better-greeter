#include <cstdint>
#include <print>

#include "greeter/greeter.hh"


auto
main( int32_t p_argc, char **p_argv ) -> int
{
    // for (const auto &p : better::Greeter::get_users())
    //     std::println("{}", p);
    better::Greeter greeter {};
    return greeter.run();
}