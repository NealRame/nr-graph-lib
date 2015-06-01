/// Point.h
///
/// - Created on: Jun 9, 2013
/// -     Author: jux
#pragma once

namespace com {
namespace nealrame {
namespace graph {
class Point {
public:
    Point()
        : _x(0)
        , _y(0) {
    }
    Point(double x, double y)
        : _x(x)
        , _y(y) {
    }
    virtual ~Point() {
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
    Point operator+(const Point &point) const {
        return Point(_x + point._x, _y + point._y);
    }

    Point & operator+=(const Point &point) {
        _x += point._x;
        _y += point._y;
        return *this;
    }

    Point operator-(const Point &point) const {
        return Point(_x - point._x, _y - point._y);
    }

    Point & operator-=(const Point &point) {
        _x -= point._x;
        _y -= point._y;
        return *this;
    }

    bool operator==(const Point &point) const {
        return this->_x == point._x && this->_y == point._y;
    }

    bool operator!=(const Point &point) const {
        return !(*this == point);
    }

public:
    void translate(double dx, double dy) {
        *this += Point(dx, dy);
    }

    Point translated(double dx, double dy) const {
        Point p(*this);
        p += Point(dx, dy);
        return p;
    }
private:
    double _x;
    double _y;
};
} // namespace graph
} // namespace nealrame
} // namespace com
