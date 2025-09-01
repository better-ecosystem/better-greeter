#include <filesystem>
#include <algorithm>
#include <fstream>
#include "greeter/session.hh"

using greeter::Session;
namespace fs = std::filesystem;


namespace
{
    [[nodiscard]]
    auto
    get_PATH( void ) -> std::vector<fs::path>
    {
        const char *PATH { std::getenv("PATH") };
        if (PATH == nullptr) return {};

        std::vector<fs::path> paths;

        std::istringstream iss { PATH };
        for (std::string val; std::getline(iss, val, ':');)
            if (!val.empty()) paths.emplace_back(val);

        return paths;
    }


    [[nodiscard]]
    auto
    get_full_bin_path( const std::string &p_bin ) -> fs::path
    {
        std::vector<fs::path> paths { get_PATH() };

        for (const fs::path &path : paths) {
            fs::path bin_path { path / p_bin };

            if (fs::exists(bin_path)) return bin_path;
        }

        return {};
    }


    [[nodiscard]]
    auto
    string_to_vec( const std::string &p_str,
                   const char        &p_delim ) -> std::vector<std::string>
    {
        std::vector<std::string> strings;
        std::istringstream iss { p_str };

        strings.reserve(std::ranges::count(p_str, p_delim));
        for (std::string val; std::getline(iss, val, p_delim);)
            strings.emplace_back(val);

        return strings;
    }
}


Session::Session( const std::string &p_bin )
{
    if (!p_bin.contains(' ')) {
        args.emplace_back(get_full_bin_path(p_bin));
        return;
    }

    size_t i      { 0 };
    auto   tokens { string_to_vec(p_bin, ' ') };
    if (tokens.front() == "env" || tokens.front().contains('=')) {
        if (tokens.front() == "env") i = 1;
        for (; i < tokens.size() && tokens[i].contains('='); i++)
            envs.emplace_back(tokens[i]);
    }

    args.emplace_back(get_full_bin_path(tokens.at(i + 1)));
    args.insert(args.end(), tokens.begin() + i + 2, tokens.end());
}


Session::Session( void )
{}


void
Session::add_args( const std::vector<std::string> &p_args )
{ args.insert(args.end(), p_args.begin(), p_args.end()); }


void
Session::add_envs( const std::vector<std::string> &p_envs )
{ envs.insert(envs.end(), p_envs.begin(), p_envs.end()); }


auto
Session::get_sessions() -> std::vector<Session>
{
    std::vector<Session> sessions;

    for (const fs::path path : SESSION_PATHS) {
        if (!fs::exists(path)) continue;

        fs::directory_iterator it { path };

        for (const auto &file : it) {
            if (!fs::is_regular_file(file.path())) [[unlikely]] continue;

            fs::path file_path { file };
            if (file_path.extension() != ".desktop") [[unlikely]] continue;

            std::ifstream session_stream { file_path };

            for (std::string line; std::getline(session_stream, line);)
                if (line.rfind("Exec=", 0) == 0)
                    sessions.emplace_back(line.substr(5));

            if (!sessions.empty() && path.string().contains('x'))
                sessions.back().X11 = true;
        }
    }

    return sessions;
}