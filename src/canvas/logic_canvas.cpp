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
    // enable necessary events
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
    add_events(Gdk::POINTER_MOTION_MASK);

}

LogicCanvas::~LogicCanvas()
{
}

void LogicCanvas::force_redraw()
{
    // get window ref
    auto win = get_window();
    if (win)    // make sure window ref is valid
    {
        // invalidate the window region containing the canvas to make it being redrawn
        auto alc = get_allocation();
        Gdk::Rectangle r(0, 0, alc.get_width(), alc.get_height());  // xy coordinates are relative to widget position (not window position)
        win->invalidate_rect(r, false);
    }
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

    // draw example lines
    cr->set_source_rgb(0.8, 0.0, 0.0);
    if (line_active)
        cr->set_source_rgb(0.0, 0.8, 0.0);
    cr->move_to(10, 10);
    cr->line_to(xc, yc );
    cr->line_to(10, height - 10);
    cr->move_to(xc, yc);
    cr->line_to(width -10, yc);
    cr->stroke();

    // draw placed lines
    cr->set_source_rgb(0.8, 0.8, 0.0);
    cr->set_line_width(4);
    for (const auto &line : m_lines)
    {
        cr->move_to(line.x1, line.y1);
        cr->line_to(line.x2, line.y2);
    }
    cr->stroke();

    // draw current line
    if (line_active)
    {
        cr->set_source_rgb(0.8, 0.0, 0.8);
        cr->move_to(m_current_line.x1, m_current_line.y1);
        cr->line_to(m_current_line.x2, m_current_line.y2);
        cr->stroke();
    }

    return true;
}

bool LogicCanvas::on_button_press_event(GdkEventButton *_e)
{
    //std::cout << "D: " << _e->x << " " << _e->y << " " << _e->button << "\n";
    switch (_e->button)
    {
        case 1: // left mouse button
            if (line_active)
            {
                // save as endpoint of last line
                m_current_line.x2 = _e->x;
                m_current_line.y2 = _e->y;
                // add line to permanent lines
                m_lines.push_back(m_current_line);
                // start new line at current coordinates
                m_current_line.x1 = _e->x;
                m_current_line.y1 = _e->y;
                m_current_line.x2 = _e->x;
                m_current_line.y2 = _e->y;
                // update window
                force_redraw();
            }
            else
            {
                // start a line at the current position
                line_active = true;
                m_current_line.x1 = _e->x;
                m_current_line.y1 = _e->y;
                m_current_line.x2 = _e->x;
                m_current_line.y2 = _e->y;
                // update window
                force_redraw();
            }
            break;

        case 3: // right mouse button
            if (line_active)
            {
                // save as endpoint of last line
                m_current_line.x2 = _e->x;
                m_current_line.y2 = _e->y;
                // add line to permanent lines
                m_lines.push_back(m_current_line);
                // disable line drawing (don't start new line)
                line_active = false;
                force_redraw();
            }
            break;
        
        default:
            break;
    }


    return true;
}

bool LogicCanvas::on_button_release_event(GdkEventButton *_e)
{
    return true;
}
bool LogicCanvas::on_motion_notify_event(GdkEventMotion *_e)
{
    // update the active line if needed
    if (line_active)
    {
        m_current_line.x2 = _e->x;
        m_current_line.y2 = _e->y;
        force_redraw();
    }
    return true;
}