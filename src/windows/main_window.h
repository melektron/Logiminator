/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
22.12.22, 14:07

Main window of the simulator containing the logic canvas,
drawing tools, ...
*/

#pragma once

#include <gtkmm/window.h>
#include <gtkmm/box.h>

#include "canvas/logic_canvas.h"


class MainWindow : public Gtk::Window
{
public:
    MainWindow();
    virtual ~MainWindow();

protected:
    // == Signal Handlers


    // == Member widgets
    Gtk::Box m_l1_columns;  // level 1 columns
    
    LogicCanvas m_canvas;

    // == UI state
};