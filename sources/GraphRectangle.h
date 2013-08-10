/*
 * GraphRectangle.h
 *
 *  Created on: Jun 9, 2013
 *      Author: jux
 */

#ifndef GRAPHRECTANGLE_H_
#define GRAPHRECTANGLE_H_

#include <vector>

#include "GraphPoint.h"
#include "GraphSize.h"

namespace com {
namespace nealrame {
namespace graph {
class Point;
class Size;
class Rectangle {
public:
	Rectangle();
	Rectangle(const Point &topLeft, const Point &bottomRight);
	Rectangle(const Point &topLeft, const Size &size);
	virtual ~Rectangle();

public:
	/**
	 * Returns true if and only if this rectangle contains the given
	 * point.
	 */
	bool contains(const Point &) const;

	/**
	 * Returns true if and only if this rectangle points are equals.
	 */
	bool isNull() const { return _top_left == _bottom_right; }

	/**
	 * Returns true if and only if this rectangle size is Valid.
	 * @See com::nealrame::graph::Size::isValid();
	 */
	bool isValid() const;

public:
	void normalize();
	void intersect(const Rectangle &);
	void unit(const Rectangle &);

public:
	/**
	 * Returns the size of this rectangle.
	 */
	Size size() const;

	/**
	 * Returns the x-coordinate of the rectangle's right edge.
	 */
	double right() const { return _bottom_right.x(); }

	/**
	 * Returns the y-coordinate of the rectangle's bottom edge.
	 */
	double bottom() const { return _bottom_right.y(); }

	/**
	 * Returns the x-coordinate of the rectangle's left edge.
	 */
	double left() const { return _top_left.x(); }

	/**
	 * Returns the y-coordinate of the rectangle's top edge.
	 */
	double top() const { return _top_left.y(); }

	/**
	 * Return the top left corner point of this rectangle.
	 */
	Point topLeft() const { return _top_left; }

	/**
	 * Return the top right corner point of this rectangle.
	 */
	Point topRight() const;

	/**
	 * Return the bottom right corner point of this rectangle.
	 */
	Point bottomRight() const { return _bottom_right; }

	/**
	 * Return the bottom left corner point of this rectangle.
	 */
	Point bottomLeft() const;

	/**
	 * Return a vector of this rectangle point in that order:
	 *   top left > top right > bottom right > bottom left.
	 */
	std::vector<Point> points() const;

public:
	/**
	 * Return the normalized version of this rectangle.
	 */
	Rectangle normalized() const;
	/**
	 * Return the result of the intersection between this rectangle and
	 * a given other one.
	 */
	Rectangle intersected(const Rectangle &) const;
	/**
	 * Return the result of the union between this rectangle and a given
	 * other one.
	 */
	Rectangle united(const Rectangle &) const;

public:
	Rectangle operator|(const Rectangle &) const;
	Rectangle operator&(const Rectangle &) const;

private:
	Point _top_left;
	Point _bottom_right;
};

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
#endif /* GRAPHRECTANGLE_H_ */