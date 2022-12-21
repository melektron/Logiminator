/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
19.12.22, 23:25

hello world windows
*/

#pragma once

#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>


class WidgetExamples : public Gtk::Window
{
public:
    WidgetExamples();
    virtual ~WidgetExamples();

protected:
    // == Signal Handlers
    void on_button_change();

    // == Member widgets
    Gtk::Box m_container;
    Gtk::Label m_infolabel;
    Gtk::Button m_getinfo_button;
    Gtk::ToggleButton m_toggle;
    Gtk::CheckButton m_check;
    Gtk::RadioButton::Group m_rbgroup;
    Gtk::RadioButton m_rb1, m_rb2, m_rb3;

    // == UI state
    int counter = 0;
};