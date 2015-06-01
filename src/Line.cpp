/*
 * Line.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: jux
 */

#include <NRGraph/Line.h>

namespace com {
namespace nealrame {
namespace graph {

Line::Line() {
}

Line::Line(const Point &p1, const Point &p2)
    : _p1(p1)
    , _p2(p2) {
}

Line::Line(double x1, double y1, double x2, double y2)
    : _p1(Point(x1, y1))
    , _p2(Point(x2, y2)) {
}

Line::~Line() {
}

double Line::dx() const {
    return _p2.x() - _p1.x();
}

double Line::dy() const {
    return _p2.y() - _p1.y();
}

void Line::translate(const Point &offset) {
    _p1 += offset;
    _p2 += offset;
}

void Line::translate(double dx, double dy) {
    _p1.x() += dx; _p2.x() += dx;
    _p1.y() += dy; _p2.y() += dy;
}

Line Line::translated(const Point &offset) const {
    Line l(*this);
    l.translate(offset);
    return l;
}

Line Line::translated(double dx, double dy) const {
    Line l(*this);
    l.translate(dx, dy);
    return l;
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
