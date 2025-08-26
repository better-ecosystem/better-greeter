#include <cstring>
#include "greeter/app.hh"
#include "greeter/log.hh"


auto
main( int p_argc, char **p_argv ) -> int
{
    if (std::strcmp(p_argv[1], "lockscreen") == 0) {
        greeter::log::write<greeter::ERROR>(
            "lockscreen mode is still WIP.");
        return 1;
    } else
        return greeter::App { }.run(p_argc, p_argv);
}