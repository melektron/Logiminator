/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
22.12.22, 14:09

Main window of the simulator containing the logic canvas,
drawing tools, ...
*/

#include <iostream>
#include "main_window.h"

MainWindow::MainWindow()
    : m_l1_columns(Gtk::ORIENTATION_HORIZONTAL)
{
    set_title("Logiminator");

    set_border_width(10);

    add(m_l1_columns);
    m_l1_columns.pack_start(m_canvas);
    m_canvas.grab_focus();

    // display the button
    show_all_children();
}

MainWindow::~MainWindow()
{
}