#include <vector>
#include <fstream>
#include <ostream>
#include <string>

#include "mainWindow.h"
#include "addStreamDialog.h"
#include "livestreamerProcess.h"
#include "fileHelper.h"

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

    readDataFile();

    auto addHandler = [this](){
        addStreamDialog dlg(*this);
        if(dlg.run() == Gtk::RESPONSE_OK) {
            Gtk::TreeModel::Row row = *(listModel->append());
            row[columns.streamUrl] = dlg.UrlEntry.get_text();
            row[columns.streamQuality] = dlg.qualityEntry.get_text();

            writeDataFile();
        }
    };
    addStreamButton->signal_clicked().connect(addHandler);

    auto removeHandler = [this](){
        using namespace Gtk;
        using namespace Glib;

        TreeModel::iterator iter = streamList->get_selection()->get_selected();

        if(iter != nullptr) {
            listModel->erase(iter);
            writeDataFile();
        }
    };
    removeStreamButton->signal_clicked().connect(removeHandler);

    auto playHandler = [this](){
        using namespace Gtk;
        using namespace Glib;

        TreeModel::iterator iter = streamList->get_selection()->get_selected();

        if(iter != nullptr) {
            TreeModel::Row row = *iter;

            // using raw pointers because livestreamerProcess will kill itself when needed
            livestreamerProcess* proc = new livestreamerProcess(row[columns.streamUrl], row[columns.streamQuality]);


            proc->addOutputWatch([this, proc](IOCondition condition) -> bool {
                Glib::ustring str, output;
                proc->output->read_line(str);
                // remove livestreamer's newline.
                //str.erase(std::remove(str.begin(), str.end(), '\n'), str.end()); -- doesn't work, "lvalue required as left operand of assignment in stl_algo.h"

                for (size_t i=0; i < str.size(); i++) {
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

    auto activatedHandler = [this] (auto path, auto unused) {
        using namespace Gtk;

        TreeModel::iterator iter = listModel->get_iter(path);

        if(iter != nullptr) {
            TreeModel::Row row = *iter;

            addStreamDialog dlg(*this, row[columns.streamUrl], row[columns.streamQuality]);

            if(dlg.run() == Gtk::RESPONSE_OK) {
                row[columns.streamUrl] = dlg.UrlEntry.get_text();
                row[columns.streamQuality] = dlg.qualityEntry.get_text();

                this->writeDataFile(); // not quite sure why I need this this->
            }
        }
    };

   streamList->signal_row_activated().connect(activatedHandler);
}

void mainWindow::readDataFile()
{
    using namespace Glib;
    using namespace std;

    ifstream stream(fileHelper::getConfigFilePath("streams.list"));

    for (string line; getline(stream, line);) { // I'd use a ustring here if getline was compatible with it
        if(!line.empty()) {
            auto separator = line.find_first_of(";");

            if(separator == ustring::npos) {
                continue;
            }

            ustring stream = line.substr(0, separator);
            ustring quality = line.substr(separator + 1, line.length() - separator);

            Gtk::TreeModel::Row row = *(listModel->append());
            row[columns.streamUrl] = stream;
            row[columns.streamQuality] = quality;
        }
    }
}

void mainWindow::writeDataFile()
{
    using namespace std;

    ofstream stream(fileHelper::getConfigFilePath("streams.list") , ofstream::out | ofstream::trunc);

    for (auto row : listModel->children()) {
        stream << row[columns.streamUrl] << ";" << row[columns.streamQuality] << endl;
    }
}
