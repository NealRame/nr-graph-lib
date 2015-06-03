/*
 * GraphRectangle.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: jux
 */

#include <NRGraph/Rectangle.h>
#include <NRGraph/Size.h>

#include <iostream>
#include <cmath>

using namespace com::nealrame::graph;

rectangle::rectangle() {
}

rectangle::rectangle(const point &topLeft, const point &bottomRight)
    : _top_left(topLeft)
    , _bottom_right(bottomRight) {
}

rectangle::rectangle(const point &topLeft, const class size &size)
    : _top_left(topLeft)
    , _bottom_right(_top_left.translated(size.width(), size.height())) {
}

rectangle::~rectangle() {
}

bool rectangle::contains(const point &point) const {
    double x = point.x(), y = point.y();
    return x >= left() && x <= right() && y >= top() && y <= bottom();
}

bool rectangle::is_valid() const {
    return size().is_valid();
}

void rectangle::normalize() {
    if (! is_valid()) {
        double min_x = _top_left.x(), min_y = _top_left.y();
        double max_x = _bottom_right.x(), max_y = _bottom_right.y();
        for (auto p: points()) {
            min_x = fmin(min_x, p.x());
            min_y = fmin(min_y, p.y());
            max_x = fmax(max_x, p.x());
            max_y = fmax(max_y, p.y());
        }
        *this = rectangle(point(min_x, min_y), point(max_x, max_y));
    }
}

void rectangle::intersect(const rectangle &other) {
    double max_top    = fmax(top(), other.top());
    double max_left   = fmax(left(), other.left());
    double min_bottom = fmin(bottom(), other.bottom());
    double min_right  = fmin(right(), other.right());

    *this = (max_left > min_right || max_top > min_bottom) ?
            rectangle() :
            rectangle(point(max_left, max_top), point(min_right, min_bottom));
}

void rectangle::unit(const rectangle &other) {
    double min_top    = fmax(top(), other.top());
    double min_left   = fmin(left(), other.left());
    double max_bottom = fmin(bottom(), other.bottom());
    double max_right  = fmax(right(), other.right());

    *this = rectangle(point(min_left, min_top), point(max_right, max_bottom));
}

size rectangle::size() const {
    return (class size){
        _bottom_right.x() - _top_left.x(),
        _bottom_right.y() - _top_left.y()
    };
}

point rectangle::top_right() const {
    return point(_bottom_right.x(), _top_left.y());
}

point rectangle::bottom_left() const {
    return point(_top_left.x(), _bottom_right.y());
}

std::vector<point> rectangle::points() const {
    std::vector<point> v;
    v.push_back(top_left());
    v.push_back(top_right());
    v.push_back(bottom_right());
    v.push_back(bottom_left());
    return v;
}

rectangle rectangle::normalized() const {
    rectangle rect(*this);
    rect.normalize();
    return rect;
}

rectangle rectangle::intersected(const rectangle &rectangle) const {
    auto rect = *this;
    rect.intersect(rectangle);
    return rect;
}

rectangle rectangle::united(const rectangle &rectangle) const {
    auto rect = *this;
    rect.unit(rectangle);
    return rect;
}

rectangle rectangle::operator|(const rectangle &rectangle) const {
    return united(rectangle);
}

rectangle rectangle::operator&(const rectangle &rectangle) const {
    return intersected(rectangle);
}
