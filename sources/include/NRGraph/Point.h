/// point.h
///
/// - Created on: Jun 9, 2013
/// -     Author: jux
#pragma once

namespace com {
namespace nealrame {
namespace graph {
class point {
public:
    point()
        : _x(0)
        , _y(0) {
    }
    point(double x, double y)
        : _x(x)
        , _y(y) {
    }
    virtual ~point() {
    }

public:
    inline double & x()
    { return _x; }
    
    inline const double & x() const
    { return _x; }
    
    inline double & y()
    { return _y; }
    
    inline const double & y() const
    { return _y; }

public:
    point operator+(const point &rhs) const {
        return point(_x + rhs._x, _y + rhs._y);
    }

    point & operator+=(const point &rhs) {
        _x += rhs._x;
        _y += rhs._y;
        return *this;
    }

    point operator-(const point &rhs) const {
        return point(_x - rhs._x, _y - rhs._y);
    }

    point & operator-=(const point &rhs) {
        _x -= rhs._x;
        _y -= rhs._y;
        return *this;
    }

    bool operator==(const point &rhs) const {
        return this->_x == rhs._x && this->_y == rhs._y;
    }

    bool operator!=(const point &rhs) const {
        return !(*this == rhs);
    }

public:
    void translate(double dx, double dy) {
        *this += point(dx, dy);
    }

    point translated(double dx, double dy) const {
        point p(*this);
        p += point(dx, dy);
        return p;
    }
private:
    double _x;
    double _y;
};
} // namespace graph
} // namespace nealrame
} // namespace com
