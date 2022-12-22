/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
22.12.22, 14:00

Drawing area Widget responsible for drawing and showing the logic circuit
*/


#pragma once

#include <gtkmm/drawingarea.h>
#include <vector>



class LogicCanvas : public Gtk::DrawingArea
{
public:
    LogicCanvas();
    virtual ~LogicCanvas();

    // forces the entire area to be redrawn by invalidating its's window allocation
    void force_redraw();

protected:
    // == signal handlers
    // mouse button events
    virtual bool on_button_press_event(GdkEventButton *_e) override;
    virtual bool on_button_release_event(GdkEventButton *_e) override;
    virtual bool on_motion_notify_event(GdkEventMotion *_e) override;

    // == drawing routine 
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& _cr) override;

    // == data and state
    // flag whether a line has started by clicking
    bool line_active = false;

    // Structure representing a line
    struct LogicLine
    {
        double x1, y1, x2, y2;
    };

    // line that is currently being drawn
    LogicLine m_current_line;

    // list of all lines that have been drawn
    std::vector<LogicLine> m_lines;

};