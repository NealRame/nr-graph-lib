/// line.h
///
/// - Created on: Jun 9, 2013
/// -     Author: jux
#pragma once

#include <NRGraph/point.h>

namespace com {
namespace nealrame {
namespace graph {
class line {
public:
    line();
    line(const point &p1, const point &p2);
    line(double x1, double y1, double x2, double y2);
    virtual ~line();

public:
    bool is_null() const
    { return _p1 == _p2; }

public:
    point & p1()
    { return _p1; }
    
    const point & p1() const
    { return _p1; }
    
    point & p2()
    { return _p2; }
    
    const point & p2() const
    { return _p2; }

    double dx() const;
    double dy() const;

    void translate(const point &offset);
    void translate(double dx, double dy);
    line translated(const point &offset) const;
    line translated(double dx, double dy) const;

public:
    bool operator==(const line &line) const {
        return this->_p1 == line._p1 && this->_p2 == line._p2;
    }
    bool operator!=(const line &line) const {
        return ! (*this == line);
    }

private:
    point _p1;
    point _p2;
};
} // namespace graph
} // namespace nealrame
} // namespace com
