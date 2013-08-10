/*
 * GraphRectangle.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: jux
 */

#include <iostream>
#include <cmath>

#include "GraphRectangle.h"

namespace com {
namespace nealrame {
namespace graph {

Rectangle::Rectangle() {
}

Rectangle::Rectangle(const Point &topLeft, const Point &bottomRight) :
	_top_left(topLeft),
	_bottom_right(bottomRight) {
}

Rectangle::Rectangle(const Point &topLeft, const Size &size) :
	_top_left(topLeft),
	_bottom_right(_top_left.translated(size.width(), size.height())) {
}

Rectangle::~Rectangle() {
}

bool Rectangle::contains(const Point &point) const {
	double x = point.x(), y = point.y();
	return x >= left() && x <= right() && y >= top() && y <= bottom();
}

bool Rectangle::isValid() const {
	return size().isValid();
}

void Rectangle::normalize() {
	if (! isValid()) {
		double min_x = _top_left.x(), min_y = _top_left.y();
		double max_x = _bottom_right.x(), max_y = _bottom_right.y();
		for (auto p: points()) {
			min_x = fmin(min_x, p.x());
			min_y = fmin(min_y, p.y());
			max_x = fmax(max_x, p.x());
			max_y = fmax(max_y, p.y());
		}
		*this = Rectangle(Point(min_x, min_y), Point(max_x, max_y));
	}
}

void Rectangle::intersect(const Rectangle &other) {
	double max_top    = fmax(top(), other.top());
	double max_left   = fmax(left(), other.left());
	double min_bottom = fmin(bottom(), other.bottom());
	double min_right  = fmin(right(), other.right());

	*this = (max_left > min_right || max_top > min_bottom) ?
			Rectangle() :
			Rectangle(Point(max_left, max_top), Point(min_right, min_bottom));
}

void Rectangle::unit(const Rectangle &other) {
	double min_top    = fmax(top(), other.top());
	double min_left   = fmin(left(), other.left());
	double max_bottom = fmin(bottom(), other.bottom());
	double max_right  = fmax(right(), other.right());

	*this = Rectangle(Point(min_left, min_top), Point(max_right, max_bottom));
}

Size Rectangle::size() const {
	return Size(_bottom_right.x() - _top_left.x(),
			_bottom_right.y() - _top_left.y());
}

Point Rectangle::topRight() const {
	return Point(_bottom_right.x(), _top_left.y());
}

Point Rectangle::bottomLeft() const {
	return Point(_top_left.x(), _bottom_right.y());
}

std::vector<Point> Rectangle::points() const {
	std::vector<Point> v;
	v.push_back(topLeft());
	v.push_back(topRight());
	v.push_back(bottomRight());
	v.push_back(bottomLeft());
	return v;
}

Rectangle Rectangle::normalized() const {
	Rectangle rect(*this);
	rect.normalize();
	return rect;
}

Rectangle Rectangle::intersected(const Rectangle &rectangle) const {
	Rectangle rect(*this);
	rect.intersect(rectangle);
	return rect;
}

Rectangle Rectangle::united(const Rectangle &rectangle) const {
	Rectangle rect(*this);
	rect.unit(rectangle);
	return rect;
}

Rectangle Rectangle::operator|(const Rectangle &rectangle) const {
	return united(rectangle);
}

Rectangle Rectangle::operator&(const Rectangle &rectangle) const {
	return intersected(rectangle);
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
