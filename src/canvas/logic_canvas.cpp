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
    cr->set_line_width(4);

    bool is_first_line = true;
    Pair2 last_line;
    for (const auto &line : m_lines)
    {
        cr->set_source_rgb(0.8, 0.8, 0.0);
        cr->move_to(XY(line.first));
        cr->line_to(XY(line.second));
        cr->stroke();

        // don't run the next part for the first line, as there
        // is no previous line to reference
        if (is_first_line) 
        {
            is_first_line = false;
            last_line = line;
            continue;
        }
        

        Vec2 last_length = last_line.second - last_line.first;
        Vec2 new_length = line.second - line.first;

        // the portion of the line that is rounded
        const double line_bezier_portion = 0.3;

        /*
        draw cubic bezier curve connecting the two lines. 
        The start point at 70% of the last line, 
        the first control point on the end of the last line.
        The second control point at the start of the new line,
        the endpoint 30% into the new line.
        */
        cr->set_source_rgb(0.0, 0.4, 0.8);
        cr->move_to(XY(last_line.second - last_length * 0.3));
        cr->curve_to(
            XY(last_line.second),
            XY(line.first),
            XY(line.first + new_length * 0.3)
        );
        cr->stroke();

        std::cout << "last length x" << last_length.getX() << " y" << last_length.getY() << " third x" << (last_length * 0.3).getX() << " y" << (last_length * 0.3).getY() << "\n";

        last_line = line;
    }

    // draw current line
    if (line_active)
    {
        cr->set_source_rgb(0.8, 0.0, 0.8);
        cr->move_to(XY(m_current_line.first));
        cr->line_to(XY(m_current_line.second));
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
                m_current_line.second.setXY(_e->x, _e->y);
                // add line to permanent lines
                m_lines.push_back(m_current_line);
                // start new line at current coordinates
                m_current_line.first.setXY(_e->x, _e->y);
                m_current_line.second.setXY(_e->x, _e->y);
                // update window
                force_redraw();
            }
            else
            {
                // start a line at the current position
                line_active = true;
                m_current_line.first.setXY(_e->x, _e->y);
                m_current_line.second.setXY(_e->x, _e->y);
                // update window
                force_redraw();
            }
            break;

        case 3: // right mouse button
            if (line_active)
            {
                // save as endpoint of last line
                m_current_line.second.setXY(_e->x, _e->y);
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
        m_current_line.second.setXY(_e->x, _e->y);
        force_redraw();
    }
    return true;
}