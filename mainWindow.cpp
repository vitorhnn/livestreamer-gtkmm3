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
    builder->get_widget("editButton", editStreamButton);
    builder->get_widget("removeButton", removeStreamButton);
    builder->get_widget("playButton", playStreamButton);
    builder->get_widget("quitButton", quitButton);
    builder->get_widget("streamList", streamList);
    builder->get_widget("statusbar", statusbar);


    listModel = Gtk::ListStore::create(columns);
    streamList->set_model(listModel);

    streamList->append_column("Name", columns.streamName);
    streamList->append_column("URL", columns.streamUrl);
    streamList->append_column("Quality", columns.streamQuality);

    readDataFile();

    auto addHandler = [this]() {
        addStreamDialog dlg(*this);
        if (dlg.run() == Gtk::RESPONSE_OK) {
            Gtk::TreeModel::Row row = *(listModel->append());
            row[columns.streamName] = dlg.nameEntry.get_text();
            row[columns.streamUrl] = dlg.UrlEntry.get_text();
            row[columns.streamQuality] = dlg.qualityEntry.get_text();

            writeDataFile();
        }
    };
    addStreamButton->signal_clicked().connect(addHandler);

    auto removeHandler = [this]() {
        using namespace Gtk;
        using namespace Glib;

        TreeModel::iterator iter = streamList->get_selection()->get_selected();

        if (iter != nullptr) {
            listModel->erase(iter);
            writeDataFile();
        }
    };
    removeStreamButton->signal_clicked().connect(removeHandler);

    auto playHandler = [this]() {
        using namespace Gtk;
        using namespace Glib;

        TreeModel::iterator iter = streamList->get_selection()->get_selected();

        if (iter != nullptr) {
            TreeModel::Row row = *iter;

            playStream(row[columns.streamUrl], row[columns.streamQuality]);
        }
    };
    playStreamButton->signal_clicked().connect(playHandler);

    auto quitHandler = [this]() {
        close();
    };
    quitButton->signal_clicked().connect(quitHandler);

    auto editHandler = [this] () {
        using namespace Gtk;

        TreeModel::iterator iter = streamList->get_selection()->get_selected();

        if (iter != nullptr) {
            TreeModel::Row row = *iter;

            addStreamDialog dlg(*this, row[columns.streamName], row[columns.streamUrl], row[columns.streamQuality]);

            if (dlg.run() == Gtk::RESPONSE_OK) {
                row[columns.streamName] = dlg.nameEntry.get_text();
                row[columns.streamUrl] = dlg.UrlEntry.get_text();
                row[columns.streamQuality] = dlg.qualityEntry.get_text();

                writeDataFile();
            }
        }
    };
    editStreamButton->signal_clicked().connect(editHandler);

    auto activatedHandler = [this] (const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* unused) {
        using namespace Gtk;

        TreeModel::iterator iter = listModel->get_iter(path);

        if (iter != nullptr) {
            TreeModel::Row row = *iter;

            playStream(row[columns.streamUrl], row[columns.streamQuality]);
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
        if (!line.empty()) {
            auto nameSeparator = line.find(";", 0);
            auto urlSeparator = line.find(";", nameSeparator + 1);


            if (nameSeparator == string::npos || urlSeparator == string::npos) {
                continue;
            }

            ustring name = line.substr(0, nameSeparator);
            ustring url = line.substr(nameSeparator + 1, urlSeparator - nameSeparator - 1);
            ustring quality = line.substr(urlSeparator + 1, line.length() - urlSeparator);

            Gtk::TreeModel::Row row = *(listModel->append());
            row[columns.streamName] = name;
            row[columns.streamUrl] = url;
            row[columns.streamQuality] = quality;
        }
    }
}

void mainWindow::writeDataFile()
{
    using namespace std;

    ofstream stream(fileHelper::getConfigFilePath("streams.list") , ofstream::out | ofstream::trunc);

    for (const auto &row : listModel->children()) {
        stream << row[columns.streamName] << ";" << row[columns.streamUrl] << ";" << row[columns.streamQuality] << endl;
    }
}

void mainWindow::playStream(const Glib::ustring &url, const Glib::ustring &quality)
{
    livestreamerProcess* proc = new livestreamerProcess(url, quality);

    proc->addOutputWatch([this, proc] (Glib::IOCondition condition) -> bool {
        Glib::ustring str, output;
        proc->output->read_line(str);
        // remove livestreamer's newline.
        //str.erase(std::remove(str.begin(), str.end(), '\n'), str.end()); -- doesn't work, "lvalue required as left operand of assignment in stl_algo.h"

        for (size_t i=0; i < str.size(); i++) {
            if (str[i] != '\n') {
                output += str[i];
            }
        }

        statusbar->push(output);

        return true;
    });
}
