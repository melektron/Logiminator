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

class HelloWorld : public Gtk::Window
{
public:
    HelloWorld();
    virtual ~HelloWorld();

protected:
    // == Signal Handlers
    void on_button_clicked();

    // == Member widgets
    Gtk::Button m_hibutton;
};