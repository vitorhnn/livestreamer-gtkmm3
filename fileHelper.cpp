#include <glibmm/refptr.h>
#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>
#include <giomm/init.h>
#include <giomm/file.h>
#include "fileHelper.h"

#include <iostream>

// this is definitely not win32 compatible

namespace fileHelper {

std::string duckTapegetenv(const std::string &variable)
{
    // so, at least on Arch Linux with glibmm 2.46.2-1, Glib::getenv(const std::string&) returns 0x0
    // which crashes the whole thing, since you can't build std::strings from 0x0.
    // so we wrap around the overload that actually works.

    // 2015/12/13: Glib::getenv(const std::string&) seems to be working with glibmm 2.46.3 now.

    bool found; // hey look I'm useless!
    return Glib::getenv(variable, found);
}

void checkForOurDirectories()
{
    using namespace std;
    using namespace Glib;
    using namespace Gio;

    Gio::init();


    // check for XDG_CONFIG_HOME
    string env = duckTapegetenv("XDG_CONFIG_HOME");
    if(!env.empty()) {
        string path = env + "/livestreamer-gtkmm3/";

        if(!file_test(path, FileTest::FILE_TEST_EXISTS)) {
            RefPtr<File> f = File::create_for_path(path);
            f->make_directory();
        }
    } else {
        // check ~/.config/livestreamer-gtkmm3/
        string path = get_home_dir() + "/.config/livestreamer-gtkmm3/";

        if(!file_test(path, FileTest::FILE_TEST_EXISTS)) {
            RefPtr<File> f = File::create_for_path(path);
            f->make_directory();
        }
    }



}

std::string getConfigFilePath(const std::string &fileName)
{
    using namespace std;
    using namespace Glib;
    // first, look for the file in XDG_CONFIG_HOME/livestreamer-gtkmm3
    // if that isn't set, use ~/.config/livestreamer-gtkmm3

    string env = duckTapegetenv("XDG_CONFIG_HOME");
    if(!env.empty()) {
        return env + "/livestreamer-gtkmm3/" + fileName;
    }

    return get_home_dir() + "/.config/livestreamer-gtkmm3/"  + fileName;
}

std::string getDataFilePath(const std::string &fileName)
{
    using namespace std;
    using namespace Glib;
    // if this is a debug build, look for the file in the current directory
    // else, look for XDG_DATA_HOME/livestreamer-gtkmm3
    // if that isn't set, check for ~/.local/share/livestreamer-gtkmm3

#ifndef NDEBUG
    return "./" + fileName;
#endif

    string env = duckTapegetenv("XDG_DATA_HOME");

    if(!env.empty()) {
        string path = env + "/livestreamer-gtkmm3/" + fileName;

        if(file_test(path, FileTest::FILE_TEST_EXISTS)) {
            return path;
        }
    } else {
        string path = get_home_dir() + "/.local/share/livestreamer-gtkmm3/" + fileName;
        if(file_test(path, FileTest::FILE_TEST_EXISTS)) {
            return path;
        }
    }

    // if everything fails, try this.
    string path = "/usr/share/livestreamer-gtkmm3/" + fileName;
    if(file_test(path, FileTest::FILE_TEST_EXISTS)) {
        return path;
    }

    // if that fails, give up
    throw std::runtime_error("Could not find any data directory, check your installation.");
}

}
