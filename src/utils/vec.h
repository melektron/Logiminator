/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
23.12.22, 12:36

Various vector structures for representing mathematical
and geometrical vectors as well as other shapes consisting of multiple
points
*/

#pragma once

// A cartesian coordinate (x and y values)
struct Cart
{
    double x = 0, y = 0;
    Cart() = default;
    Cart(double _x, double _y);
};

// A polar coordinate (r and phi)
struct Polar
{
    double r = 0, phi = 0;
    Polar() = default;
    Polar(double _r, double _phi);
};

/**
 * @brief A vector representing a location or distance that allows seamless
 * conversion between polar and cartesian coordinates
 *
 */
struct Vec2
{
protected:
    Cart m_cartesian;
    Polar m_polar;

    // updates contained polar value from cartesian value
    void update_polar_from_cart();
    // updates contained cartesian value from polar value
    void update_cart_from_polar();

public:
    // default constructor init all to 0
    Vec2() = default;
    // initialize from cartesian coordinate
    Vec2(Cart _value);
    // initialize from polar coordinate
    Vec2(Polar _value);
    // creates Vec2 from raw cartesian values
    static Vec2 fromXY(double _x, double _y);
    // creates Vec2 from raw polar values
    static Vec2 fromRPhi(double _r, double _phi);

    // might be required for inheritance
    virtual ~Vec2();

    // == raw value getters
    double getX() const;
    double getY() const;
    double getR() const;
    double getPhi() const;

    // == raw value setters
    void setX(double _x);
    void setY(double _y);
    void setXY(double _x, double _y);
    void setR(double _r);
    void setPhi(double _phi);
    void setRPhi(double _r, double _phi);

    // == coordinate setters
    void setCart(const Cart &_value);
    void setRad(const Polar &_value);

    // == conversions to coordinate types
    const Cart &getCart() const;
    const Polar &getRad() const;
    operator Cart() const;
    operator Polar() const;

    // == mathematical operations
    // = addition and subtraction
    // vector addition: separately adds x and y values
    Vec2 operator+(const Vec2 &_rhs) const;
    // scalar addition: increases the vector's length by the given number
    Vec2 operator+(double _rhs) const;
    // numeric addition: adds the length of the vector to the number
    friend double operator+(double _lhs, const Vec2 &_rhs);
    // vector subtraction: separately subtracts x and y values
    Vec2 operator-(const Vec2 &_rhs) const;
    // scalar subtraction: reduces the vector's length by the given number
    Vec2 operator-(double _rhs) const;
    // numeric subtraction: subtracts the length of the vector from the number
    friend double operator-(double _lhs, const Vec2 &_rhs);
    // = multiplication and division
    // scalar multiplication: multiplies the length of the vector with the number
    Vec2 operator*(double _rhs) const;
    // numeric multiplication: multiplies the number with the length of the vector
    friend double operator*(double _lhs, const Vec2 &_rhs);
    // scalar division: divides the length of the vector by the number
    Vec2 operator/(double _rhs) const;
    // numeric division: divides the number by the length of the vector
    friend double operator/(double _lhs, const Vec2 &_rhs);
};
