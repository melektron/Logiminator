/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
23.12.22, 22:52

custom bezier functions adding to the functionality 
cairo provides
*/

#pragma once

#include <cairomm/context.h>
#include "utils/vec.h"

namespace bezier
{
    /**
     * @brief creates a cubic bezier path on the cairo context _cr. This is equivalent to the
     * Cairo::Context::curve_to function, in fact that is what is being used to draw the curve.
     * The curve_to function is just a cubic bezier function. 
     * This function simply rearranges the arguments slightly, using vectors for coordinates and also 
     * including the start coordinate (so no move_to() required before this function).
     * Apart from that, this function is completely identical to the curve_to function and was only added
     * for completeness.
     * 
     * @param _cr cairo drawing context to apply the curve to
     * @param _p1 start point
     * @param _c1 first control point
     * @param _c2 second control point
     * @param _p1 end point
     */
    void cubic(const Cairo::RefPtr<Cairo::Context> &_cr, const Vec2 &_p1, const Vec2 &_c1, const Vec2 &_c2, const Vec2 &_p2);

    /**
     * @brief draws a quadratic bezier curve by arranging the two control points of a cubic bezier curve
     * so that it is equivalent to a quadratic one. Mathematically, this is true any time the vector
     * P1 -> P2 is 3 time the vector C1 -> C2. So their lengths have to be in the right proportions (3:1)
     * and they both have to point in the same direction (more info: https://math.stackexchange.com/questions/2759306/can-a-cubic-bezier-curve-be-a-quadratic-one-if-two-control-points-are-equal-from)
     * 
     * Experimenting with curves: https://www.desmos.com/calculator/d1ofwre0fr?lang=fr
     * 
     * @param _cr cairo drawing context to apply the curve to
     * @param _p1 start point
     * @param _c  control point
     * @param _p1 end point
     */
    void quadratic(const Cairo::RefPtr<Cairo::Context> &_cr, const Vec2 &_p1, const Vec2 &_c, const Vec2 &_p2);
}