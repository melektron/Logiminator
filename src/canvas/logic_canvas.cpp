/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
22.12.22, 13:59

Drawing area Widget responsible for drawing and showing the logic circuit
*/

#include <cairomm/context.h>
#include <gdkmm/event.h>
#include <iostream>
#include "logic_canvas.h"
#include "utils/bezier.h"

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
    if (win) // make sure window ref is valid
    {
        // invalidate the window region containing the canvas to make it being redrawn
        auto alc = get_allocation();
        Gdk::Rectangle r(0, 0, alc.get_width(), alc.get_height()); // xy coordinates are relative to widget position (not window position)
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
    cr->line_to(xc, yc);
    cr->line_to(10, height - 10);
    cr->move_to(xc, yc);
    cr->line_to(width - 10, yc);
    cr->stroke();

    bool is_first_line = true;
    Pair2 last_line;
    int segment_count = 0; // line segment count to check for last segment
    for (const auto &line : m_lines)
    {
        // increase line segment count
        segment_count++;

        // maximum the relative portion of the line that is rounded when the line is short
        const double line_bezier_portion = 0.3;
        // the absolute amount of pixels of the line that are rounded when the line is long
        const double max_bezier_width = 30; // px
        // calculate the proportional values
        Vec2 last_bezier_width = (last_line.second - last_line.first) * line_bezier_portion;
        Vec2 new_bezier_width = (line.second - line.first) * line_bezier_portion;
        // constrain the proportional bezier width to the maximum value
        last_bezier_width.setR(std::min(last_bezier_width.getR(), max_bezier_width));
        new_bezier_width.setR(std::min(new_bezier_width.getR(), max_bezier_width));

        // draw the line
        cr->set_line_width(4);
        cr->set_source_rgb(0.8, 0.8, 0.0);
        // the first line doesn't start with a bezier curve
        if (is_first_line)
            cr->move_to(XY(line.first));
        else
            cr->move_to(XY(line.first + new_bezier_width));
        // the last line doesn't end with a bezier curve
        if (segment_count == m_lines.size())
            cr->line_to(XY(line.second));
        else
            cr->line_to(XY(line.second - new_bezier_width));
        cr->stroke();
        // the first line doesn't need to draw a connection to the last line, so it ends here
        if (is_first_line)
        {
            is_first_line = false;
            last_line = line;
            continue;
        }

        // connect the lines with a quadratic bezier curve
        cr->set_source_rgb(1.0, 0.3, 0.0);
        bezier::quadratic(
            cr,
            last_line.second - last_bezier_width,
            last_line.second,
            line.first + new_bezier_width);
        cr->stroke();

        last_line = line;
    }

    return true;
}

bool LogicCanvas::on_button_press_event(GdkEventButton *_e)
{
    // std::cout << "D: " << _e->x << " " << _e->y << " " << _e->button << "\n";
    switch (_e->button)
    {
    case 1: // left mouse button
        if (line_active)
        {
            // save as endpoint of last line
            m_lines.back().second.setXY(_e->x, _e->y);
            // add new line
            m_lines.emplace_back();
            // start new line at current coordinates
            m_lines.back().first.setXY(_e->x, _e->y);
            m_lines.back().second.setXY(_e->x, _e->y);
            // update window
            force_redraw();
        }
        else
        {
            // start a line at the current position
            line_active = true;
            m_lines.emplace_back();
            m_lines.back().first.setXY(_e->x, _e->y);
            m_lines.back().second.setXY(_e->x, _e->y);
            // update window
            force_redraw();
        }
        break;

    case 3: // right mouse button
        if (line_active)
        {
            // save as endpoint of last line
            m_lines.back().second.setXY(_e->x, _e->y);
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
        m_lines.back().second.setXY(_e->x, _e->y);
        force_redraw();
    }
    return true;
}