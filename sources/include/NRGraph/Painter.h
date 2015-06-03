/// GraphPainter.h
///
/// - Created on: 11 juin 2013
/// -     Author: jux
#pragma once

#include <NRGraph/point.h>

#include <memory>

namespace com {
namespace nealrame {
namespace graph {
class brush;
class color;
class line;
class pen;
class Rectangle;
class Surface;
class painter {
public:
    enum class Antialias {
        None,
        Gray,
        Subpixel,
    };

public:
    painter(Surface &);
    virtual ~painter();

public:
    Surface & surface();
    const Surface & surface() const
    { return const_cast<painter *>(this)->surface(); }

public:
    void set_antialiasing(Antialias);

public:
    void clear(const color &);

public:
    void save();
    void restore();

public:
    class brush brush() const;
    void set_brush(const class brush &);

    class pen pen() const;
    void set_pen(const class pen &);

public:
    void draw_line(double x0, double y0, double x1, double y1);
    void draw_line(const point &, const point &);
    void draw_line(const line &);
    void draw_rectangle(const Rectangle &);
    void fill_rectangle(const Rectangle &);

private:
    struct impl;
    std::unique_ptr<impl> d;
};
} // namespace graph
} // namespace nealrame
} // namespace com