#include <vector>
#include <algorithm>

#include "mainWindow.h"
#include "addStreamDialog.h"
#include "livestreamerProcess.h"

mainWindow::mainWindow(BaseObjectType *base, const Glib::RefPtr<Gtk::Builder> &builder) :
    Gtk::Window(base),
    builder(builder)
{
    builder->get_widget("addButton", addStreamButton);
    builder->get_widget("removeButton", removeStreamButton);
    builder->get_widget("playButton", playStreamButton);
    builder->get_widget("quitButton", quitButton);
    builder->get_widget("streamList", streamList);
    builder->get_widget("statusbar", statusbar);


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

            // using raw pointers because livestreamerProcess will kill itself when needed
            livestreamerProcess* proc = new livestreamerProcess(argv);


            proc->addOutputWatch([this, proc](IOCondition condition) -> bool {
                Glib::ustring str, output;
                proc->output->read_line(str);
                // remove livestreamer's newline.
                //str.erase(std::remove(str.begin(), str.end(), '\n'), str.end()); -- doesn't work, "lvalue required as left operand of assignment in stl_algo.h"

                for(size_t i=0; i< str.size(); i++)
                {
                    if(str[i] != '\n') {
                        output += str[i];
                    }
                }

                statusbar->push(output);

                return true;
            });
        }
    };
    playStreamButton->signal_clicked().connect(playHandler);

    auto quitHandler = [this](){
        close();
    };
    quitButton->signal_clicked().connect(quitHandler);
}
