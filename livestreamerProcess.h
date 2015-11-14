#ifndef LIVESTREAMER_PROCESS_H
#define LIVESTREAMER_PROCESS_H

#include <vector>

#include <glibmm/main.h>
#include <glibmm/spawn.h>
#include <glibmm/iochannel.h>
#include <glibmm/ustring.h>

#include <sigc++/slot.h>

class livestreamerProcess
{
public:
    livestreamerProcess(std::vector<Glib::ustring>& argv);
    ~livestreamerProcess();

    // no copy
    livestreamerProcess(const livestreamerProcess&) = delete;
    livestreamerProcess& operator=(const livestreamerProcess&) = delete;

    void addOutputWatch(const sigc::slot<bool, Glib::IOCondition> &slot);
    Glib::RefPtr<Glib::IOChannel>     output;
private:
    Glib::Pid                         pid;
    int                               stdin;
    int                               stdout;
    int                               stderr;
};

#endif // LIVESTREAMER_PROCESS_H
