/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
23.12.22, 22:52

custom bezier functions adding to the functionality
cairo provides
*/

#include "bezier.h"

void bezier::cubic(const Cairo::RefPtr<Cairo::Context> &_cr, const Vec2 &_p1, const Vec2 &_c1, const Vec2 &_c2, const Vec2 &_p2)
{
    _cr->move_to(XY(_p1));
    _cr->curve_to(XY(_c1), XY(_c2), XY(_p2));
}

void bezier::quadratic(const Cairo::RefPtr<Cairo::Context> &_cr, const Vec2 &_p1, const Vec2 &_c, const Vec2 &_p2)
{
    // 1st control point is two thirds or the way from p1 to c
    Vec2 c1 = _p1 + (_c - _p1) * (2.0/3.0);
    // 2nd control point is two thirds of the way from p2 to c
    Vec2 c2 = _p2 + (_c - _p2) * (2.0/3.0);

    cubic(_cr, _p1, c1, c2, _p2);
}