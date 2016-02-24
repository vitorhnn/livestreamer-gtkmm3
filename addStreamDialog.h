#ifndef ADDSTREAMDIALOG_H
#define ADDSTREAMDIALOG_H

#include <glibmm/ustring.h>

#include <gtkmm/dialog.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/box.h>


class addStreamDialog : public Gtk::Dialog {
public:
    addStreamDialog(Gtk::Window& parent, const Glib::ustring &name = "Bob Ross", const Glib::ustring &url = "twitch.tv/bobross", const Glib::ustring &quality = "best");

    Gtk::Entry      nameEntry;
    Gtk::Entry      UrlEntry;
    Gtk::Entry      qualityEntry;
private:
    Gtk::Box        hBox;
    Gtk::Button*    acceptButton;
    Gtk::Button*    cancelButton;
};



#endif// ADDSTREAMDIALOG_H
