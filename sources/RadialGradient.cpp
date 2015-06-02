/*
 * GraphRadialGradient.cpp
 *
 *  Created on: 13 juin 2013
 *      Author: jux
 */

#include <NRGraph/RadialGradient.h>
#include <NRGraph/Color.h>
#include <NRGraph/Point.h>

extern "C" {
#   include <cairo.h>
}

#include <cassert>
#include <vector>
#include <string>

using namespace com::nealrame::graph;

RadialGradient::RadialGradient(const Point &center, double radius)
    : _start_circle_center(center)
    , _start_circle_radius(radius)
    , _end_circle_center(center)
    , _end_circle_radius(radius) {
}

RadialGradient::RadialGradient(const Point &startCenter, double startRadius, const Point &endCenter, double endRadius)
    : _start_circle_center(startCenter)
    , _start_circle_radius(startRadius)
    , _end_circle_center(endCenter)
    , _end_circle_radius(endRadius) {
}

RadialGradient::RadialGradient(const RadialGradient &other)
    : gradient(other)
    , _start_circle_center(other.startCircleCenterPoint())
    , _start_circle_radius(other.startCircleRadius())
    , _end_circle_center(other.endCircleCenterPoint())
    , _end_circle_radius(other.endCircleRadius()) {
}

RadialGradient::RadialGradient(const void *ptr) {
    auto pattern = reinterpret_cast<cairo_pattern_t *>((void *)ptr);

    assert(::cairo_pattern_get_type(pattern) == CAIRO_PATTERN_TYPE_RADIAL);

    double x1, y1, x2, y2, r1, r2;
    ::cairo_pattern_get_radial_circles(pattern, &x1, &y1, &r1, &x2, &y2, &r2);
    setStartCircleCenterPoint(Point{x1, y1});
    setStartCircleRadius(r1);
    setEndCircleCenterPoint(Point{x2, y2});
    setEndCircleRadius(r2);

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

RadialGradient::~RadialGradient() {
}

std::shared_ptr<void> RadialGradient::pattern_() const {
    const auto c1 = startCircleCenterPoint(), c2 = endCircleCenterPoint();
    const auto r1 = startCircleRadius(), r2 = endCircleRadius();

    auto pattern = cairo_pattern_create_radial(c1.x(), c1.y(), r1, c2.x(), c2.y(), r2);

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
        ::cairo_pattern_add_color_stop_rgba(pattern, stop.offset, rgb.red, rgb.green, rgb.blue, stop.color.alpha());
    }

    return std::shared_ptr<void>(pattern, ::cairo_pattern_destroy);
}

RadialGradient & RadialGradient::operator=(const gradient &gradient) {
    gradient::operator=(gradient);
    return *this;
}

std::string RadialGradient::typeToString() const {
    return "RadialGradient";
}
