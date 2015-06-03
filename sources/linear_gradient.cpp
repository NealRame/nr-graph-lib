/*
 * linear_gradient.cpp
 *
 *  Created on: 13 juin 2013
 *      Author: jux
 */

#include <NRGraph/linear_gradient.h>
#include <NRGraph/color.h>
#include <NRGraph/point.h>

extern "C" {
#   include <cairo.h>
}

#include <cassert>
#include <vector>
#include <string>

using namespace com::nealrame::graph;

linear_gradient::linear_gradient() {
}

linear_gradient::linear_gradient(const point &start, const point &end)
    : _start_point(start)
    , _end_point(end) {
}

linear_gradient::linear_gradient(const linear_gradient &other)
    : gradient(other)
    , _start_point(other._start_point)
    , _end_point(other._end_point) {
}

linear_gradient::linear_gradient(const void *ptr) {
    auto pattern = reinterpret_cast<cairo_pattern_t *>((void *)ptr);

    assert(::cairo_pattern_get_type(pattern) == CAIRO_PATTERN_TYPE_LINEAR);

    double x1, y1, x2, y2;
    ::cairo_pattern_get_linear_points(pattern, &x1, &y1, &x2, &y2);
    set_start_point(point{x1, y1});
    set_end_point(point{x2, y2});

    switch(::cairo_pattern_get_extend(pattern)) {
    case CAIRO_EXTEND_NONE:
        set_extend(extend::None);
        break;

    case CAIRO_EXTEND_PAD:
        set_extend(extend::Pad);
        break;

    case CAIRO_EXTEND_REFLECT:
        set_extend(extend::Reflect);
        break;

    case CAIRO_EXTEND_REPEAT:
        set_extend(extend::Repeat);
        break;
    }

    auto stop_count = 0;
    ::cairo_pattern_get_color_stop_count(pattern, &stop_count);
    for (auto i = 0; i < stop_count; ++i) {
        double offset, r, g, b, a;
        ::cairo_pattern_get_color_stop_rgba(pattern, i, &offset, &r, &g, &b, &a);
        add_color_stop(stop{offset, color(color::RGB{r, g, b}, a)});
    }
}

linear_gradient::~linear_gradient() {
}

std::shared_ptr<void> linear_gradient::pattern_() const {
    const auto p1 = start_point(), p2 = end_point();
    auto pattern = cairo_pattern_create_linear(p1.x(), p1.y(), p2.x(), p2.y());

    switch (extend()) {
    case extend::None:
        cairo_pattern_set_extend(pattern, CAIRO_EXTEND_NONE);
        break;
    case extend::Pad:
        cairo_pattern_set_extend(pattern, CAIRO_EXTEND_PAD);
        break;
    case extend::Reflect:
        cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REFLECT);
        break;
    case extend::Repeat:
        cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REPEAT);
        break;
    }
    for (const auto &stop : color_stops()) {
        auto rgb = stop.color.rgb();
        cairo_pattern_add_color_stop_rgba(pattern, stop.offset, rgb.red, rgb.green, rgb.blue, stop.color.alpha());
    }

    return std::shared_ptr<void>(pattern, ::cairo_pattern_destroy);
}

linear_gradient & linear_gradient::operator=(const class gradient &rhs) {
    gradient::operator=(rhs);
    return *this;
}

std::string linear_gradient::type_to_string() const {
    return "linear_gradient";
}
