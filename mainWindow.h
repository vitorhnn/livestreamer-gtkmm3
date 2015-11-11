#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/liststore.h>
#include <glibmm/ustring.h>
#include <glibmm/refptr.h>


class mainWindow : public Gtk::Window {
public:
    mainWindow(BaseObjectType* type, const Glib::RefPtr<Gtk::Builder>& builder);
private:

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
    Gtk::ToolButton*            addStreamButton;
    Gtk::ToolButton*            playStreamButton;
    Gtk::ToolButton*            removeStreamButton;
    Gtk::ToolButton*            quitButton;

    Gtk::TreeView*              streamList;


    ModelColumns columns;
    Glib::RefPtr<Gtk::ListStore> listModel;
};



#endif // MAINWINDOW_H
