#include <glibmm/refptr.h>

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/messagedialog.h>

#include "mainWindow.h"
#include "fileHelper.h"

int main(int argc, char* argv[])
{
    using namespace Gtk;
#ifdef NDEBUG
    try {
#endif
        fileHelper::checkForOurDirectories();
        Glib::RefPtr<Application> app = Application::create(argc, argv, "br.net.vitorhnn.livestreamer-gtkmm3");

        Glib::RefPtr<Builder> builder = Builder::create();

        builder->add_from_file(fileHelper::getDataFilePath("./UI/mainWindow.glade"));

        mainWindow* window = nullptr;
        builder->get_widget_derived("MainWindow", window);
        app->run(*window);

        delete window;
#ifdef NDEBUG
    } catch(const std::exception& ex) {
        MessageDialog dlg("Uncaught exception", false, MESSAGE_ERROR);
        dlg.set_secondary_text(ex.what());
        dlg.run();
        return 1;
    } catch(const Glib::Exception& ex) { // For whatever reason, Glib::Exception does not inherit from std::exception
        MessageDialog dlg("Uncaught exception", false, MESSAGE_ERROR);
        dlg.set_secondary_text(ex.what());
        dlg.run();
        return 1;
    }
#endif

    return 0;
}
