/*
 * GraphRadialGradient.cpp
 *
 *  Created on: 13 juin 2013
 *      Author: jux
 */

#include <NRGraph/RadialGradient.h>
#include <NRGraph/Color.h>
#include <NRGraph/point.h>

extern "C" {
#   include <cairo.h>
}

#include <cassert>
#include <vector>
#include <string>

using namespace com::nealrame::graph;

radial_gradient::radial_gradient(const point &center, double radius)
    : _start_circle_center(center)
    , _start_circle_radius(radius)
    , _end_circle_center(center)
    , _end_circle_radius(radius) {
}

radial_gradient::radial_gradient(const point &startCenter, double startRadius, const point &endCenter, double endRadius)
    : _start_circle_center(startCenter)
    , _start_circle_radius(startRadius)
    , _end_circle_center(endCenter)
    , _end_circle_radius(endRadius) {
}

radial_gradient::radial_gradient(const radial_gradient &other)
    : gradient(other)
    , _start_circle_center(other.start_circle_center_point())
    , _start_circle_radius(other.start_circle_radius())
    , _end_circle_center(other.end_circle_center_point())
    , _end_circle_radius(other.end_circle_radius()) {
}

radial_gradient::radial_gradient(const void *ptr) {
    auto pattern = reinterpret_cast<cairo_pattern_t *>((void *)ptr);

    assert(::cairo_pattern_get_type(pattern) == CAIRO_PATTERN_TYPE_RADIAL);

    double x1, y1, x2, y2, r1, r2;
    ::cairo_pattern_get_radial_circles(pattern, &x1, &y1, &r1, &x2, &y2, &r2);
    set_start_circle_center_point(point{x1, y1});
    set_start_circle_radius(r1);
    set_end_circle_center_point(point{x2, y2});
    set_end_circle_radius(r2);

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

radial_gradient::~radial_gradient() {
}

std::shared_ptr<void> radial_gradient::pattern_() const {
    const auto c1 = start_circle_center_point(), c2 = end_circle_center_point();
    const auto r1 = start_circle_radius(), r2 = end_circle_radius();

    auto pattern = cairo_pattern_create_radial(c1.x(), c1.y(), r1, c2.x(), c2.y(), r2);

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
        ::cairo_pattern_add_color_stop_rgba(pattern, stop.offset, rgb.red, rgb.green, rgb.blue, stop.color.alpha());
    }

    return std::shared_ptr<void>(pattern, ::cairo_pattern_destroy);
}

radial_gradient & radial_gradient::operator=(const gradient &gradient) {
    gradient::operator=(gradient);
    return *this;
}

std::string radial_gradient::type_to_string() const {
    return "radial_gradient";
}
