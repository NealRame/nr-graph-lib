/*
 * GraphPainter.cpp
 *
 *  Created on: 11 juin 2013
 *      Author: jux
 */

#include <NRGraph/Painter.h>
#include <NRGraph/Brush.h>
#include <NRGraph/Error.h>
#include <NRGraph/Line.h>
#include <NRGraph/Pen.h>
#include <NRGraph/Point.h>
#include <NRGraph/Rectangle.h>
#include <NRGraph/Size.h>
#include <NRGraph/Surface.h>

extern "C" {
#   include <cairo.h>
}

#include <vector>

#define CAIRO_SURFACE(NR_SURFACE) reinterpret_cast<cairo_surface_t *>((NR_SURFACE)._priv_data)
#define CAIRO_CONTEXT(PTR) reinterpret_cast<cairo_t  *>(PTR)
#define CAIRO_PATTERN(PTR) reinterpret_cast<cairo_pattern_t *>(PTR)

using namespace com::nealrame::graph;

Painter::Painter(Surface &surface) : _surface(surface) {
    cairo_t *context = cairo_create(CAIRO_SURFACE(surface));

    cairo_status_t status;
    if ((status = cairo_status(context)) != CAIRO_STATUS_SUCCESS) {
        Error::raise(Error::InternalError, cairo_status_to_string(status));
    }

    _priv_data = context;
}

Painter::~Painter() {
    cairo_destroy(CAIRO_CONTEXT(_priv_data));
}

void Painter::setAntialiasing(Antialias antialias) {
    cairo_t *cr =   CAIRO_CONTEXT(_priv_data);
    switch (antialias) {
    case Antialias::None:
        cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
        break;

    case Antialias::Gray:
        cairo_set_antialias(cr, CAIRO_ANTIALIAS_GRAY);
        break;

    case Antialias::Subpixel:
        cairo_set_antialias(cr, CAIRO_ANTIALIAS_SUBPIXEL);
        break;
    }
}

void Painter::clear(const Color &color) {
    save();
    setBrush(color);
    fillRectangle(Rectangle(Point(0, 0), _surface.size()));
    restore();
}

void Painter::save() {
    cairo_save(CAIRO_CONTEXT(_priv_data));
}

void Painter::restore() {
    cairo_restore(CAIRO_CONTEXT(_priv_data));
}

Brush Painter::brush() const {
    Brush b;
    b.importFromCairoPattern(static_cast<void *>(cairo_get_source(CAIRO_CONTEXT(_priv_data))));
    return b;
}

void Painter::setBrush(const Brush &brush) {
    cairo_set_source(CAIRO_CONTEXT(_priv_data), CAIRO_PATTERN(brush.exportToCairoPattern()));
}

Pen Painter::pen() const {
    cairo_t *cr = CAIRO_CONTEXT(_priv_data);
    Pen pen;
    pen.setWidth(cairo_get_line_width(cr));
    return pen;
}

void Painter::setPen(const Pen &pen) {
    cairo_t *cr = CAIRO_CONTEXT(_priv_data);
    cairo_set_line_width(cr, pen.width());
}

void Painter::drawLine(double x0, double y0, double x1, double y1) {
    cairo_t *cr = CAIRO_CONTEXT(_priv_data);
    cairo_move_to(cr, x0, y0);
    cairo_line_to(cr, x1, y1);
    cairo_stroke(cr);
}

void Painter::drawLine(const Point &a, const Point &b) {
    cairo_t *cr = CAIRO_CONTEXT(_priv_data);
    cairo_move_to(cr, a.x(), a.y());
    cairo_line_to(cr, b.x(), b.y());
    cairo_stroke(cr);
}

void Painter::drawLine(const Line &line) {
    cairo_t *cr = CAIRO_CONTEXT(_priv_data);
    Point a(line.p1()), b(line.p2());
    cairo_move_to(cr, a.x(), a.y());
    cairo_line_to(cr, b.x(), b.y());
    cairo_stroke(cr);
}

void Painter::drawRectangle(const Rectangle &rectangle) {
    cairo_t *cr = CAIRO_CONTEXT(_priv_data);
    if (rectangle.isValid()) {
        Point point = rectangle.topLeft();
        Size size = rectangle.size();
        cairo_rectangle(cr, point.x(), point.y(), size.width(), size.height());
        cairo_stroke(cr);
    }
}

void Painter::fillRectangle(const Rectangle &rectangle) {
    cairo_t *cr = CAIRO_CONTEXT(_priv_data);
    if (rectangle.isValid()) {
        Point point = rectangle.topLeft();
        Size size = rectangle.size();
        cairo_rectangle(cr, point.x(), point.y(), size.width(), size.height());
        cairo_fill(cr);
    }
}
