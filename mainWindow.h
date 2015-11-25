#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <glibmm/ustring.h>
#include <glibmm/refptr.h>

#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/liststore.h>
#include <gtkmm/statusbar.h>

class mainWindow : public Gtk::Window {
public:
    mainWindow(BaseObjectType* type, const Glib::RefPtr<Gtk::Builder>& builder);
private:

    void readDataFile();
    void writeDataFile();

    class ModelColumns : public Gtk::TreeModelColumnRecord {
    public:
        ModelColumns()
        {
            add(streamUrl);
            add(streamQuality);
        }
        Gtk::TreeModelColumn<Glib::ustring> streamUrl;
        Gtk::TreeModelColumn<Glib::ustring> streamQuality;
    };

    Glib::RefPtr<Gtk::Builder>  builder;

    // TOOLBAR ITEMS
    Gtk::ToolButton*            addStreamButton;
    Gtk::ToolButton*            playStreamButton;
    Gtk::ToolButton*            removeStreamButton;
    Gtk::ToolButton*            quitButton;

    Gtk::TreeView*              streamList;

    Gtk::Statusbar*             statusbar;


    ModelColumns columns;
    Glib::RefPtr<Gtk::ListStore> listModel;
};



#endif // MAINWINDOW_H
