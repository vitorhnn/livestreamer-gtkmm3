#include <gtkmm.h>
#include "mainWindow.h"

using namespace Gtk;

int main(int argc, char* argv[])
{
    Glib::RefPtr<Application> app = Application::create(argc, argv, "br.net.vitorhnn.livestreamerui");

    Glib::RefPtr<Builder> builder = Builder::create();
    try
    {
        builder->add_from_file("./UI/mainWindow.glade");
    }
    catch(const std::exception& ex) { /* oh noes! */ }

    mainWindow* window = nullptr;
    builder->get_widget_derived("MainWindow", window);
    app->run(*window);

    delete window;
    
    return 0;
}
