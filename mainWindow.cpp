#include <vector>
#include <glibmm/spawn.h>

#include "mainWindow.h"
#include "addStreamDialog.h"


mainWindow::mainWindow(BaseObjectType *base, const Glib::RefPtr<Gtk::Builder> &builder) :
    Gtk::Window(base),
    builder(builder)
{
    builder->get_widget("addButton", addStreamButton);
    builder->get_widget("removeButton", removeStreamButton);
    builder->get_widget("playButton", playStreamButton);
    builder->get_widget("quitButton", quitButton);
    builder->get_widget("streamList", streamList);


    listModel = Gtk::ListStore::create(columns);
    streamList->set_model(listModel);

    streamList->append_column("URL", columns.streamUrl);
    streamList->append_column("Quality", columns.streamQuality);


    auto addHandler = [this](){
        addStreamDialog dlg(*this);
        if(dlg.run() == Gtk::RESPONSE_OK) {
            Gtk::TreeModel::Row row = *(listModel->append());
            row[columns.streamUrl] = dlg.UrlEntry.get_text();
            row[columns.streamQuality] = dlg.qualityEntry.get_text();
        }
    };
    addStreamButton->signal_clicked().connect(addHandler);

    auto removeHandler = [this](){
        using namespace Gtk;
        using namespace Glib;

        TreeModel::iterator iter = streamList->get_selection()->get_selected();

        if(iter != nullptr) {
            listModel->erase(iter);
        }
    };
    removeStreamButton->signal_clicked().connect(removeHandler);

    auto playHandler = [this](){
        using namespace Gtk;
        using namespace Glib;
        using namespace std;

        TreeModel::iterator iter = streamList->get_selection()->get_selected();

        if(iter != nullptr) {
            TreeModel::Row row = *iter;
            vector<ustring> argv;
            argv.push_back("livestreamer");
            argv.push_back(row[columns.streamUrl]);
            argv.push_back(row[columns.streamQuality]);
            spawn_async("", argv, SPAWN_SEARCH_PATH); // todo: use pipes to read the output
        }
    };
    playStreamButton->signal_clicked().connect(playHandler);

    auto quitHandler = [this](){
        close();
    };
    quitButton->signal_clicked().connect(quitHandler);
}
