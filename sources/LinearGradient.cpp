/*
 * GraphLinearGradient.cpp
 *
 *  Created on: 13 juin 2013
 *      Author: jux
 */

#include <NRGraph/LinearGradient.h>
#include <NRGraph/Color.h>
#include <NRGraph/Point.h>

extern "C" {
#   include <cairo.h>
}

#include <cassert>
#include <vector>
#include <string>

using namespace com::nealrame::graph;

LinearGradient::LinearGradient() {
}

LinearGradient::LinearGradient(const Point &start, const Point &end)
    : _start_point(start)
    , _end_point(end) {
}

LinearGradient::LinearGradient(const LinearGradient &other)
    : gradient(other)
    , _start_point(other._start_point)
    , _end_point(other._end_point) {
}

LinearGradient::LinearGradient(const void *ptr) {
    auto pattern = reinterpret_cast<cairo_pattern_t *>((void *)ptr);

    assert(::cairo_pattern_get_type(pattern) == CAIRO_PATTERN_TYPE_LINEAR);

    double x1, y1, x2, y2;
    ::cairo_pattern_get_linear_points(pattern, &x1, &y1, &x2, &y2);
    setStartPoint(Point{x1, y1});
    setEndPoint(Point{x2, y2});

    switch(::cairo_pattern_get_extend(pattern)) {
    case CAIRO_EXTEND_NONE:
        set_extend(Extend::None);
        break;

    case CAIRO_EXTEND_PAD:
        set_extend(Extend::Pad);
        break;

    case CAIRO_EXTEND_REFLECT:
        set_extend(Extend::Reflect);
        break;

    case CAIRO_EXTEND_REPEAT:
        set_extend(Extend::Repeat);
        break;
    }

    auto stop_count = 0;
    ::cairo_pattern_get_color_stop_count(pattern, &stop_count);
    for (auto i = 0; i < stop_count; ++i) {
        double offset, r, g, b, a;
        ::cairo_pattern_get_color_stop_rgba(pattern, i, &offset, &r, &g, &b, &a);
        add_color_stop(Stop{offset, color(color::RGB{r, g, b}, a)});
    }
}

LinearGradient::~LinearGradient() {
}

std::shared_ptr<void> LinearGradient::pattern_() const {
    const auto p1 = startPoint(), p2 = endPoint();
    auto pattern = cairo_pattern_create_linear(p1.x(), p1.y(), p2.x(), p2.y());

    switch (extend()) {
    case Extend::None:
        cairo_pattern_set_extend(pattern, CAIRO_EXTEND_NONE);
        break;
    case Extend::Pad:
        cairo_pattern_set_extend(pattern, CAIRO_EXTEND_PAD);
        break;
    case Extend::Reflect:
        cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REFLECT);
        break;
    case Extend::Repeat:
        cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REPEAT);
        break;
    }
    for (const auto &stop : color_stops()) {
        auto rgb = stop.color.rgb();
        cairo_pattern_add_color_stop_rgba(pattern, stop.offset, rgb.red, rgb.green, rgb.blue, stop.color.alpha());
    }

    return std::shared_ptr<void>(pattern, ::cairo_pattern_destroy);
}

LinearGradient & LinearGradient::operator=(const class gradient &rhs) {
    gradient::operator=(rhs);
    return *this;
}

std::string LinearGradient::typeToString() const {
    return "LinearGradient";
}
