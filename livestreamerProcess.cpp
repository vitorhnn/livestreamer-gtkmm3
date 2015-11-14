#include "livestreamerProcess.h"
#include <iostream>
livestreamerProcess::livestreamerProcess(std::vector<Glib::ustring> &argv)
{
    using namespace Glib;

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
    Glib::spawn_close_pid(pid);
}

void livestreamerProcess::addOutputWatch(const sigc::slot< bool, Glib::IOCondition >& slot)
{
    Glib::signal_io().connect(slot, stdout, Glib::IO_IN);
}
