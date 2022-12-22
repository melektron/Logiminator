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

protected:
    // == signal handlers


    // == drawing routine 
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& _cr) override;

    bool on_button_press_event(GdkEventButton *_e);
    bool on_button_release_event(GdkEventButton *_e);

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