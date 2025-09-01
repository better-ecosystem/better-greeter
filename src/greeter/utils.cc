#include <charconv>
#include <cstring>
#include <fstream>

#include <gtkmm/picture.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>

#include "greeter/utils.hh"


namespace
{
    [[nodiscard]]
    auto
    get_PATH( void ) -> std::vector<greeter::fs::path>
    {
        const char *PATH { std::getenv("PATH") };
        if (PATH == nullptr) return {};

        std::vector<greeter::fs::path> paths;

        std::istringstream iss { PATH };
        for (std::string val; std::getline(iss, val, ':');)
            if (!val.empty()) paths.emplace_back(val);

        return paths;
    }


    [[nodiscard]]
    auto
    get_full_bin_path( const std::string &p_bin ) -> greeter::fs::path
    {
        std::vector<greeter::fs::path> paths { get_PATH() };

        for (const greeter::fs::path &path : paths) {
            greeter::fs::path bin_path { path / p_bin };

            if (greeter::fs::exists(bin_path)) return bin_path;
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


namespace greeter
{
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
                if (!fs::is_regular_file(file.path())) continue;

                fs::path file_path { file };
                if (file_path.extension() != ".desktop") continue;

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


    auto
    check_and_create_app_dir( void ) -> std::string
    {
        bool created { false };

        if (!fs::exists(BASE_PATHS[0])) try {
            fs::create_directories(BASE_PATHS[0]);
            created = true;
        } catch (const std::exception &e) { return e.what(); }

        if (!fs::exists(CACHE_PATH)) try {
            Json::Value cache;
            std::map users { get_users() };

            if (!users.empty())
                cache["username"] = Json::Value { users.begin()->first };
            else throw std::runtime_error(
                      "Failed to open /etc/passwd for reading");

            if (!write_cache(cache))
                throw std::runtime_error("Failed to write cache file");

            created = true;
        } catch (const std::exception &e) { return e.what(); }

        return created ? "created" : "";
    }


    auto
    get_app_file( std::string_view p_file ) -> fs::path
    {
        for (const std::string_view &base : BASE_PATHS) {
            fs::path path { fs::path(base) / p_file };
            if (fs::exists(path)) return path;
        }
        return fs::path();
    }


    auto
    get_users( void ) -> std::map<std::string, fs::path>
    {
        std::map<std::string, fs::path> users;

        std::ifstream passwd { "/etc/passwd" };

        /* Error returned will always come from "failure to open file",
           so theres no need to return a message / std::expected.
        */
        if (!passwd.is_open()) [[unlikely]] return users;

        std::array<std::string, 7> buffer;

        for (std::string line; std::getline(passwd, line);) {
            {
                std::istringstream iss { line };

                size_t i { 0 };
                for (std::string val; std::getline(iss, val, ':') && i < 7; i++)
                    buffer[i] = val;
            }

            /* The parsed string will contain these values: */
            /* [0] = name
               [1] = "x"
               [2] = UID
               [3] = GID
               [4] = description
               [5] = HOME
               [6] = shell
            */
            int32_t res { 0 };
            auto [ ptr, ec ] { std::from_chars(buffer[2].data(),
                                            buffer[2].data() + buffer[2].size(),
                                            res) };
            if (ec != std::errc()) continue;
            if (res < 1000) continue;

            if (buffer[0] == "nobody") continue;

            users.emplace(buffer[0], buffer[5]);
        }

        return users;
    }


    auto
    set_pfp( Gtk::Picture &p_pfp, const std::string &p_home ) -> bool
    {
        auto is_valid_face { []( const fs::path &p_path ) -> bool {
            return fs::exists(p_path) && fs::is_regular_file(p_path);
        } };


        fs::path face1 { fs::path(p_home) / ".face" };
        fs::path face2 { fs::path(p_home) / ".face.icon" };

        if (is_valid_face(face1)) {
            p_pfp.set_filename(face1.string());
            return true;
        }

        if (is_valid_face(face2)) {
            p_pfp.set_filename(face2.string());
            return true;
        }

        return false;
    }


    auto
    get_cache( void ) -> Json::Value
    {
        std::ifstream cache_file { fs::path(CACHE_PATH) };
        if (!cache_file.is_open()) [[unlikely]] return Json::nullValue;

        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;

        Json::Value root;
        std::string err;
        if (!Json::parseFromStream(builder, cache_file, &root, &err))
            [[unlikely]]
        {
            root["err"] = Json::Value { err };
            return root;
        }

        return root;
    }


    auto
    write_cache( const Json::Value &p_val ) -> bool
    {
        std::ofstream cache_file { fs::path(CACHE_PATH) };
        if (!cache_file.is_open()) [[unlikely]] return false;

        Json::StreamWriterBuilder writer;
        writer["indentation"] = "    ";
        cache_file << Json::writeString(writer, p_val);
        return true;
    }
}