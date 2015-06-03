/// GraphRectangle.h
///
/// - Created on: Jun 9, 2013
/// -     Author: jux
#pragma once

#include <NRGraph/Point.h>
#include <NRGraph/Size.h>

#include <vector>

namespace com {
namespace nealrame {
namespace graph {
class Size;
class rectangle {
public:
    rectangle();
    rectangle(const point &topLeft, const point &bottomRight);
    rectangle(const point &topLeft, const Size &size);
    virtual ~rectangle();

public:
    /**
     * Returns true if and only if this rectangle contains the given
     * point.
     */
    bool contains(const point &) const;

    /**
     * Returns true if and only if this rectangle points are equals.
     */
    bool is_null() const { return _top_left == _bottom_right; }

    /**
     * Returns true if and only if this rectangle size is Valid.
     * @See com::nealrame::graph::Size::isValid();
     */
    bool is_valid() const;

public:
    void normalize();
    void intersect(const rectangle &);
    void unit(const rectangle &);

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
    point top_left() const { return _top_left; }

    /**
     * Return the top right corner point of this rectangle.
     */
    point top_right() const;

    /**
     * Return the bottom right corner point of this rectangle.
     */
    point bottom_right() const { return _bottom_right; }

    /**
     * Return the bottom left corner point of this rectangle.
     */
    point bottom_left() const;

    /**
     * Return a vector of this rectangle point in that order:
     *   top left > top right > bottom right > bottom left.
     */
    std::vector<point> points() const;

public:
    /**
     * Return the normalized version of this rectangle.
     */
    rectangle normalized() const;
    /**
     * Return the result of the intersection between this rectangle and
     * a given other one.
     */
    rectangle intersected(const rectangle &) const;
    /**
     * Return the result of the union between this rectangle and a given
     * other one.
     */
    rectangle united(const rectangle &) const;

public:
    rectangle operator|(const rectangle &) const;
    rectangle operator&(const rectangle &) const;

private:
    point _top_left;
    point _bottom_right;
};

} // namespace graph
} // namespace nealrame
} // namespace com
