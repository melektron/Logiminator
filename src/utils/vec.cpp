/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
23.12.22, 12:36

Various vector structures for representing mathematical
and geometrical vectors as well as other shapes consisting of multiple
points
*/

#include <cmath>
#include "vec.h"

// == Cart and Polar

Cart::Cart(double _x, double _y)
    : x(_x), y(_y)
{
}

Polar::Polar(double _r, double _phi)
    : r(_r), phi(_phi)
{
}

// == Vec2

Vec2::Vec2(Cart _value)
    : m_cartesian(_value)
{
    // calculate matching polar value
    update_polar_from_cart();
}
Vec2::Vec2(Polar _value)
    : m_polar(_value)
{
    // calculate matching cartesian value
    update_cart_from_polar();
}
Vec2 Vec2::fromXY(double _x, double _y)
{
    return Cart(_x, _y); // constructs Vec2 from Cart
}
Vec2 Vec2::fromRPhi(double _r, double _phi)
{
    return Polar(_r, _phi); // constructs Vec2 from Polar
}

double Vec2::getX() const { return m_cartesian.x; }
double Vec2::getY() const { return m_cartesian.y; }
double Vec2::getR() const { return m_polar.r; }
double Vec2::getPhi() const { return m_polar.phi; }

void Vec2::setX(double _x)
{
    m_cartesian.x = _x;
    update_polar_from_cart();
}
void Vec2::setY(double _y)
{
    m_cartesian.y = _y;
    update_polar_from_cart();
}
void Vec2::setXY(double _x, double _y)
{
    m_cartesian.x = _x;
    m_cartesian.y = _y;
    update_polar_from_cart();
}
void Vec2::setR(double _r)
{
    m_polar.r = _r;
    update_cart_from_polar();
}
void Vec2::setPhi(double _phi)
{
    m_polar.phi = _phi;
    update_cart_from_polar();
}
void Vec2::setRPhi(double _r, double _phi)
{
    m_polar.r = _r;
    m_polar.phi = _phi;
    update_cart_from_polar();
}

void Vec2::setCart(const Cart &_value)
{
    m_cartesian = _value;
    update_polar_from_cart();
}
void Vec2::setRad(const Polar &_value)
{
    m_polar = _value;
    update_cart_from_polar();
}

const Cart &Vec2::getCart() const { return m_cartesian; }
const Polar &Vec2::getRad() const { return m_polar; }
Vec2::operator Cart() const { return m_cartesian; }
Vec2::operator Polar() const { return m_polar; }


Vec2 Vec2::operator+(const Vec2 &_rhs) const
{
    return Cart(
        m_cartesian.x + _rhs.m_cartesian.x,
        m_cartesian.y + _rhs.m_cartesian.y
    );
}
Vec2 Vec2::operator+(double _rhs) const
{
    return Polar(
        m_polar.r + _rhs,
        m_polar.phi
    );
}
double operator+(double _lhs, const Vec2 &_rhs) // friend
{
    return _lhs + _rhs.m_polar.r;
}
Vec2 Vec2::operator-(const Vec2 &_rhs) const
{
    return Cart(
        m_cartesian.x - _rhs.m_cartesian.x,
        m_cartesian.y - _rhs.m_cartesian.y
    );
}
Vec2 Vec2::operator-(double _rhs) const
{
    return Polar(
        m_polar.r - _rhs,
        m_polar.phi
    );
}
double operator-(double _lhs, const Vec2 &_rhs) // friend
{
    return _lhs - _rhs.m_polar.r;
}
Vec2 Vec2::operator*(double _rhs) const
{
    return Polar(
        m_polar.r * _rhs,
        m_polar.phi
    );
}
double operator*(double _lhs, const Vec2 &_rhs)
{
    return _lhs * _rhs.m_polar.r;
}
Vec2 Vec2::operator/(double _rhs) const
{
    return Polar(
        m_polar.r / _rhs,
        m_polar.phi
    );
}
double operator/(double _lhs, const Vec2 &_rhs)
{
    return _lhs / _rhs.m_polar.r;
}

void Vec2::update_polar_from_cart()
{
    m_polar.r = std::sqrt(
        std::pow(m_cartesian.x, 2) + std::pow(m_cartesian.y, 2)
    );
    m_polar.phi = std::atan(m_cartesian.y / m_cartesian.x);
}
void Vec2::update_cart_from_polar()
{
    m_cartesian.x = m_polar.r * std::cos(m_polar.phi);
    m_cartesian.y = m_polar.r * std::sin(m_polar.phi);
}