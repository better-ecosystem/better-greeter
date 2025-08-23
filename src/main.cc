#include "greeter/app.hh"


auto
main( int p_argc, char **p_argv ) -> int
{
    return greeter::App { }.run(p_argc, p_argv);
}