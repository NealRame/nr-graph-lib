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
#include <NRGraph/LinearGradient.h>
#include <NRGraph/Pen.h>
#include <NRGraph/Point.h>
#include <NRGraph/RadialGradient.h>
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

namespace {
    struct context_deleter {
        void operator()(cairo_t *context) {
            ::cairo_destroy(context);
        }
    };
}

struct Painter::impl {
    Surface &surface;
    std::unique_ptr<cairo_t, ::context_deleter> cairo_context;

    impl(Surface &surface)
        : surface(surface) {
        cairo_context.reset(
            ::cairo_create(reinterpret_cast<cairo_surface_t *>(surface.priv_data_()))
        );
        cairo_status_t status;
        if ((status = cairo_status(cairo_context.get())) != CAIRO_STATUS_SUCCESS) {
            error::raise(error::InternalError, cairo_status_to_string(status));
        }
    }

    void setAntialiasing(Antialias antialias) {
        auto context = cairo_context.get();
        switch (antialias) {
        case Antialias::None:
            cairo_set_antialias(context, CAIRO_ANTIALIAS_NONE);
            break;

        case Antialias::Gray:
            cairo_set_antialias(context, CAIRO_ANTIALIAS_GRAY);
            break;

        case Antialias::Subpixel:
            cairo_set_antialias(context, CAIRO_ANTIALIAS_SUBPIXEL);
            break;
        }
    }

    void save() {
        ::cairo_save(cairo_context.get());
    }

    void restore() {
        ::cairo_restore(cairo_context.get());
    }

    void clear(const color &c) {
        save();
        setBrush(c);
        fillRectangle(Rectangle(Point(0, 0), surface.size()));
        restore();
    }

    class brush brush() const {
        return (class brush)(::cairo_get_source(cairo_context.get()));
    }

    void setBrush(const class brush &br) {
        auto pattern = br.pattern_();
        ::cairo_set_source(cairo_context.get(), reinterpret_cast<cairo_pattern_t *>(pattern.get()));
    }

    Pen pen() const {
        Pen pen;
        pen.setWidth(::cairo_get_line_width(cairo_context.get()));
        return pen;
    }

    void setPen(const Pen &pen) {
        ::cairo_set_line_width(cairo_context.get(), pen.width());
    }

    void drawLine(double x0, double y0, double x1, double y1) {
        auto cr = cairo_context.get();
        ::cairo_move_to(cr, x0, y0);
        ::cairo_line_to(cr, x1, y1);
        ::cairo_stroke(cr);
    }

    void drawRectangle(const Rectangle &rectangle) {
        auto cr = cairo_context.get();
        if (rectangle.isValid()) {
            const auto point = rectangle.topLeft();
            const auto size = rectangle.size();
            ::cairo_rectangle(cr, point.x(), point.y(), size.width(), size.height());
            ::cairo_stroke(cr);
        }
    }

    void fillRectangle(const Rectangle &rectangle) {
        auto cr = cairo_context.get();
        if (rectangle.isValid()) {
            const auto point = rectangle.topLeft();
            const auto size = rectangle.size();
            ::cairo_rectangle(cr, point.x(), point.y(), size.width(), size.height());
            ::cairo_fill(cr);
        }
    }
};

Painter::Painter(Surface &surface)
    : d(new impl(surface)) {
}

Painter::~Painter() {
}

Surface & Painter::surface()
{ return d->surface; }

void Painter::setAntialiasing(Antialias antialias) {
    d->setAntialiasing(antialias);
}

void Painter::clear(const color &c) {
    d->clear(c);
}

void Painter::save() {
    d->save();
}

void Painter::restore() {
    d->restore();
}

class brush Painter::brush() const {
    return d->brush();
}

void Painter::setBrush(const class brush &br) {
    d->setBrush(br);
}

Pen Painter::pen() const {
    return d->pen();
}

void Painter::setPen(const Pen &pen) {
    d->setPen(pen);
}

void Painter::drawLine(double x0, double y0, double x1, double y1) {
    d->drawLine(x0, y0, x1, y1);
}

void Painter::drawLine(const Point &a, const Point &b) {
    drawLine(a.x(), a.y(), b.x(), b.y());
}

void Painter::drawLine(const Line &line) {
    drawLine(line.p1(), line.p2());
}

void Painter::drawRectangle(const Rectangle &rectangle) {
    d->drawRectangle(rectangle);
}

void Painter::fillRectangle(const Rectangle &rectangle) {
    d->fillRectangle(rectangle);
}
