#include <glibmm/refptr.h>

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/messagedialog.h>

#include "mainWindow.h"


int main(int argc, char* argv[])
{
    using namespace Gtk;
    try {
        Glib::RefPtr<Application> app = Application::create(argc, argv, "br.net.vitorhnn.livestreamer-gtkmm3");

        Glib::RefPtr<Builder> builder = Builder::create();

        builder->add_from_file("./UI/mainWindow.glade");

        mainWindow* window = nullptr;
        builder->get_widget_derived("MainWindow", window);
        app->run(*window);

        delete window;
    } catch(const std::exception& ex) {
        MessageDialog dlg("Uncaught exception", false, MESSAGE_ERROR);
        dlg.set_secondary_text(ex.what());
        dlg.run();
        return 1;
    }
    return 0;
}
