/// GraphPainter.h
///
/// - Created on: 11 juin 2013
/// -     Author: jux
#pragma once

#include <NRGraph/Point.h>

#include <memory>

namespace com {
namespace nealrame {
namespace graph {
class Brush;
class Color;
class Line;
class Pen;
class Rectangle;
class Surface;
class Painter {
public:
    enum class Antialias {
        None,
        Gray,
        Subpixel,
    };

public:
    Painter(Surface &);
    virtual ~Painter();

public:
    Surface & surface();
    const Surface & surface() const
    { return const_cast<Painter *>(this)->surface(); }

public:
    void setAntialiasing(Antialias);

public:
    void clear(const Color &);

public:
    void save();
    void restore();

public:
    Brush brush() const;
    void setBrush(const Brush &);

    Pen pen() const;
    void setPen(const Pen &);

public:
    void drawLine(double x0, double y0, double x1, double y1);
    void drawLine(const Point &, const Point &);
    void drawLine(const Line &);
    void drawRectangle(const Rectangle &);
    void fillRectangle(const Rectangle &);

private:
    struct impl;
    std::unique_ptr<impl> d;
};
} // namespace graph
} // namespace nealrame
} // namespace com