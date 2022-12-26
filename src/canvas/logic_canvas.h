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
#include "utils/vec.h"



class LogicCanvas : public Gtk::DrawingArea
{
public:
    LogicCanvas();
    virtual ~LogicCanvas();

    // forces the entire area to be redrawn by invalidating its's window allocation
    void forceRedraw();

protected:
    // == signal handlers
    // mouse button events
    virtual bool on_button_press_event(GdkEventButton *_e) override;
    virtual bool on_button_release_event(GdkEventButton *_e) override;
    // mouse move event
    virtual bool on_motion_notify_event(GdkEventMotion *_e) override;
    // scroll event
    virtual bool on_scroll_event(GdkEventScroll *_e) override;

    // == drawing routine 
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& _cr) override;

    // == helper functions
    /**
     * @brief calculates the coordinates in the rendered image matching
     * the screen coordinates relative to the DrawingArea Widget's origin
     * given the current scaling factor and view translation
     * 
     * @param _screen_coords screen coordinates
     * @return Vec2 matching render coordinates
     */
    Vec2 screenCoordsToRenderCoords(Vec2 _screen_coords) const;
    /**
     * @brief calculates the coordinates on the window matching
     * the given render coordinates relative to the DrawingArea Widget's origin
     * given the current scaling factor and view translation
     * 
     * @param _render_coords render coordinates
     * @return Vec2 matching screen coordinates
     */
    Vec2 renderCoordsToScreenCoords(Vec2 _render_coords) const;

    // == data and state
    // flag whether a line has started by clicking
    bool m_line_active = false;

    // list of all lines that have been drawn
    std::vector<Pair2> m_lines;

    // canvas translation and scale state affected by scrolling
    Vec2 m_view_translation;
    double m_view_scale = 0.5;

    // whether a view shifting operation is active. set when middle button is pressed
    // and reset when middle button is released
    bool m_view_shift_active = false;
    // render coordinate where a view shift operation was started (aka. the point that should be moved)
    Vec2 m_view_shift_starting_point;

};