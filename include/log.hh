#pragma once
#include <source_location>
#include <format>
#include <syslog.h>


namespace log
{
    struct FmtString
    {
        const char *fmt;
        std::source_location source;


        FmtString( const char *p_fmt, const std::source_location &p_source =
                                            std::source_location::current() ) :
            fmt(p_fmt), source(p_source)
        {}
    };


    enum Level : uint8_t
    {
        DEBUG = LOG_DEBUG,   /* << Debugging information */
        INFO  = LOG_INFO,    /* << General   information */
        WARN  = LOG_WARNING, /* << Non fatal errors */
        ERROR = LOG_CRIT     /* << Fatal errors */
    };


    template<typename... T_Args>
    [[nodiscard]]
    auto
    format( const char *p_fmt,
            T_Args &&...p_args ) -> std::string
    {
        std::string res { std::vformat(p_fmt,
                                       std::make_format_args(p_args...)) };
        return res.c_str();
    }


    template<Level T_LogLevel, typename... T_Args>
    void
    write( const FmtString &p_fmt, T_Args &&...p_args )
    {
        std::string msg { format(p_fmt.fmt, p_args...) };
        syslog(T_LogLevel, "%s", msg.c_str());
    }
}