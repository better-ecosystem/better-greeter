#include <cstdint>

#include "greeter/greeter.hh"


auto
main( int32_t p_argc, char **p_argv ) -> int
{
    better::Greeter greeter {};
    return greeter.run();
}