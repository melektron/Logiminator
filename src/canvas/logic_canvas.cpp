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
    add_events(Gdk::SCROLL_MASK);
}

LogicCanvas::~LogicCanvas()
{
}

void LogicCanvas::forceRedraw()
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

    // setup line styles
    cr->set_line_join(Cairo::LineJoin::LINE_JOIN_ROUND);
    cr->set_line_cap(Cairo::LineCap::LINE_CAP_ROUND);

    // set the translation
    cr->scale(m_view_scale, m_view_scale);
    cr->translate(XY(m_view_translation));


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
    Vec2 location = screenCoordsToRenderCoords(Vec2::fromXY(_e->x, _e->y));
    switch (_e->button)
    {
    case 1: // left mouse button
        if (m_line_active)
        {
            // save as endpoint of last line
            m_lines.back().second = location;
            // add new line
            m_lines.emplace_back();
            // start new line at current coordinates
            m_lines.back().first = location;
            m_lines.back().second = location;
            // update window
            forceRedraw();
        }
        else
        {
            // start a line at the current position
            m_line_active = true;
            m_lines.emplace_back();
            m_lines.back().first = location;
            m_lines.back().second = location;
            // update window
            forceRedraw();
        }
        break;

    case 3: // right mouse button
        if (m_line_active)
        {
            // save as endpoint of last line
            m_lines.back().second = location;
            // disable line drawing (don't start new line)
            m_line_active = false;
            forceRedraw();
        }
        break;
    
    case 2: // middle mouse button
        m_view_shift_active = true;
        m_view_shift_starting_point = location;
        break;

    default:
        break;
    }

    return true;
}

bool LogicCanvas::on_button_release_event(GdkEventButton *_e)
{
    Vec2 location = screenCoordsToRenderCoords(Vec2::fromXY(_e->x, _e->y));
    switch (_e->button)
    {
    case 2: // middle mouse button
        m_view_shift_active = false;
        break;
    
    default:
        break;
    }

    return true;
}
bool LogicCanvas::on_motion_notify_event(GdkEventMotion *_e)
{
    // convert to render coordinates
    Vec2 location = screenCoordsToRenderCoords(Vec2::fromXY(_e->x, _e->y));
    //printf("mouse   : x%4lf y%4lf\n", location.getX(), location.getY());

    // update the active line if needed
    if (m_line_active)
    {
        m_lines.back().second = location;
        forceRedraw();
    }
    // update view translation if needed
    else if (m_view_shift_active)
    {
        // "location" is the coordinate that the cursor is currently located at
        // "m_view_shift_starting_point" is the coordinate that should be at the cursor location.
        // we calculate the offset and add it to the translation so the error is resolved
        Vec2 correction_offset = location - m_view_shift_starting_point;
        m_view_translation += correction_offset;
        forceRedraw();
    }
    return true;
}
bool LogicCanvas::on_scroll_event(GdkEventScroll *_e)
{
    printf("scroll at x%4lf y%4lf: %1d (mods: %1d)\n", _e->x, _e->y, _e->direction, _e->state);
    switch (_e->direction)
    {
    case GdkScrollDirection::GDK_SCROLL_UP:
    {
        // get render location of the mouse that is to be preserved
        Vec2 mouse_location_before = screenCoordsToRenderCoords(Vec2::fromXY(_e->x, _e->y));
        // change the scaling factor
        m_view_scale += m_view_scale * 0.1;
        // get the render locatoin of the mouse after the operation
        Vec2 mouse_location_after = screenCoordsToRenderCoords(Vec2::fromXY(_e->x, _e->y));
        // correct the translation so the location is the same again
        m_view_translation += (mouse_location_after - mouse_location_before);
        // m_view_translation.setY(m_view_translation.getY() + 5);
    }
    break;
    case GdkScrollDirection::GDK_SCROLL_DOWN:
    {

        // get render location of the mouse that is to be preserved
        Vec2 mouse_location_before = screenCoordsToRenderCoords(Vec2::fromXY(_e->x, _e->y));
        // change the scaling factor
        m_view_scale -= m_view_scale * 0.1;
        // get the render locatoin of the mouse after the operation
        Vec2 mouse_location_after = screenCoordsToRenderCoords(Vec2::fromXY(_e->x, _e->y));
        // correct the translation so the location is the same again
        m_view_translation += (mouse_location_after - mouse_location_before);
        // m_view_translation.setY(m_view_translation.getY() - 5);
    }
    break;
    case GdkScrollDirection::GDK_SCROLL_LEFT:
        m_view_translation.setX(m_view_translation.getX() + 5);
        break;
    case GdkScrollDirection::GDK_SCROLL_RIGHT:
        m_view_translation.setX(m_view_translation.getX() - 5);
        break;

    // only redraw window if translation changed
    default:
        return true;
    }
    forceRedraw();
    return true;
}

Vec2 LogicCanvas::screenCoordsToRenderCoords(Vec2 _screen_coords) const
{
    return _screen_coords / m_view_scale - m_view_translation;
}
Vec2 LogicCanvas::renderCoordsToScreenCoords(Vec2 _render_coords) const
{
    return (_render_coords + m_view_translation) * m_view_scale;
}