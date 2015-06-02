/*
 * line.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: jux
 */

#include <NRGraph/Line.h>

namespace com {
namespace nealrame {
namespace graph {

line::line() {
}

line::line(const Point &p1, const Point &p2)
    : _p1(p1)
    , _p2(p2) {
}

line::line(double x1, double y1, double x2, double y2)
    : _p1(Point(x1, y1))
    , _p2(Point(x2, y2)) {
}

line::~line() {
}

double line::dx() const {
    return _p2.x() - _p1.x();
}

double line::dy() const {
    return _p2.y() - _p1.y();
}

void line::translate(const Point &offset) {
    _p1 += offset;
    _p2 += offset;
}

void line::translate(double dx, double dy) {
    _p1.x() += dx; _p2.x() += dx;
    _p1.y() += dy; _p2.y() += dy;
}

line line::translated(const Point &offset) const {
    line l(*this);
    l.translate(offset);
    return l;
}

line line::translated(double dx, double dy) const {
    line l(*this);
    l.translate(dx, dy);
    return l;
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
