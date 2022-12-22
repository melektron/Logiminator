/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
22.12.22, 13:59

Drawing area Widget responsible for drawing and showing the logic circuit
*/

#include "logic_canvas.h"
#include <cairomm/context.h>
#include <gdkmm/event.h>
#include <iostream>

LogicCanvas::LogicCanvas()
{
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
}

LogicCanvas::~LogicCanvas()
{
}

bool LogicCanvas::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // coordinates for the center of the window
    int xc, yc;
    xc = width / 2;
    yc = height / 2;

    cr->set_line_width(10.0);
    cr->set_line_join(Cairo::LineJoin::LINE_JOIN_ROUND);
    cr->set_line_cap(Cairo::LineCap::LINE_CAP_ROUND);

    // draw red lines out from the center of the window
    cr->set_source_rgb(0.8, 0.0, 0.0);
    if (line_active)
        cr->set_source_rgb(0.0, 0.8, 0.0);
    cr->move_to(10, 10);
    cr->line_to(xc, yc );
    cr->line_to(10, height - 10);
    cr->move_to(xc, yc);
    cr->line_to(width -10, yc);
    cr->stroke();


    return true;
}

bool LogicCanvas::on_button_press_event(GdkEventButton *_e)
{
    std::cout << "D: " << _e->x << " " << _e->y << "\n";
    return true;
}

bool LogicCanvas::on_button_release_event(GdkEventButton *_e)
{
    std::cout << "U: " << _e->x << " " << _e->y << "\n";
    return true;
}