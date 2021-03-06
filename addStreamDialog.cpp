#include <gtkmm/stock.h>

#include "addStreamDialog.h"


addStreamDialog::addStreamDialog(Gtk::Window &parent, const Glib::ustring &name, const Glib::ustring &url, const Glib::ustring &quality) :
    Gtk::Dialog("Add a stream", parent, true),
    acceptButton(add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK)),
    cancelButton(add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL))
{
    acceptButton->set_can_default();
    acceptButton->grab_default();
    set_border_width(10);

    nameEntry.set_placeholder_text("Bob Ross");
    UrlEntry.set_placeholder_text("twitch.tv/bobross");
    qualityEntry.set_placeholder_text("best");

    nameEntry.set_text(name);
    UrlEntry.set_text(url);
    qualityEntry.set_text(quality);

    nameEntry.set_activates_default();
    UrlEntry.set_activates_default();
    qualityEntry.set_activates_default();

    // GTKMM PLS: I'm adding a border to the dialog's action area using deprecated functions
    // GTKMM DEVS, pls add action-area-border
    get_action_area()->set_border_width(10);

    get_content_area()->pack_start(hBox);
    hBox.pack_start(nameEntry, Gtk::PACK_EXPAND_WIDGET, 5);
    hBox.pack_start(UrlEntry, Gtk::PACK_EXPAND_WIDGET, 5);
    hBox.pack_start(qualityEntry, Gtk::PACK_EXPAND_WIDGET, 5);
    show_all();
}
