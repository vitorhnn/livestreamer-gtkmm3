#include "livestreamerProcess.h"

#include <vector>

livestreamerProcess::livestreamerProcess(const Glib::ustring& streamUrl, const Glib::ustring& streamQuality)
{
    using namespace Glib;

    std::vector<ustring> argv;
    argv.push_back("livestreamer");
    argv.push_back(streamUrl);
    argv.push_back(streamQuality);

    spawn_async_with_pipes("",
                           argv,
                           SPAWN_SEARCH_PATH | SPAWN_DO_NOT_REAP_CHILD,
                           SlotSpawnChildSetup(),
                           &pid,
                           &stdin,
                           &stdout,
                           &stderr
                           );

    output = IOChannel::create_from_fd(stdout);

    signal_child_watch().connect([this] (auto pid, auto status){
        delete this;
    }, pid);
}

livestreamerProcess::~livestreamerProcess()
{
    for(auto connection : connections) {
        connection.disconnect();
    }
    Glib::spawn_close_pid(pid);
}

void livestreamerProcess::addOutputWatch(const sigc::slot< bool, Glib::IOCondition >& slot)
{
    connections.push_back(Glib::signal_io().connect(slot, stdout, Glib::IO_IN));
}
