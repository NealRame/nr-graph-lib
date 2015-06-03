/*
 * brush.cpp
 *
 *  Created on: 12 juin 2013
 *      Author: jux
 */

#include <NRGraph/brush.h>

extern "C" {
#   include <cairo.h>
}

#include <sstream>

#include <NRGraph/Error.h>
#include <NRGraph/LinearGradient.h>
#include <NRGraph/RadialGradient.h>

namespace com {
namespace nealrame {
namespace graph {

struct brush::impl {
    std::unique_ptr<class color> color;
    std::unique_ptr<class gradient> gradient;
};

brush::brush()
    : type_(type::Null)
    , d(new impl) {
}

brush::brush(const class color &c)
    : brush() {
    set_color(c);
}

brush::brush(const color::name name)
    : brush() {
    set_color(name);
}

brush::brush(const class graph::gradient &gradient)
    : brush() {
    set_gradient(gradient);
}

brush::brush(const brush &br)
    : brush() {
    *this = br;
}

brush::brush(brush &&brush) {
    *this = std::move(brush);
}

brush::brush(const void *ptr) {
    auto pattern = reinterpret_cast<cairo_pattern_t *>((void *)ptr);

    switch (::cairo_pattern_get_type(pattern)) {
    case CAIRO_PATTERN_TYPE_SOLID:
        set_color((class color)(ptr));
        break;

    case CAIRO_PATTERN_TYPE_LINEAR:
        set_gradient(linear_gradient(ptr));
        break;

    case CAIRO_PATTERN_TYPE_RADIAL:
        set_gradient(radial_gradient(ptr));
        break;
        
    case CAIRO_PATTERN_TYPE_SURFACE:
    case CAIRO_PATTERN_TYPE_MESH:
    case CAIRO_PATTERN_TYPE_RASTER_SOURCE:
        error::raise(error::NotImplemented); // TODO not implement yet
        break;
    }
}

brush::~brush() {
}

std::shared_ptr<void> brush::pattern_() const {
    switch (type()) {
    case type::Null:
        return nullptr;
    case type::Solid:
        return color().pattern_();
    case type::Gradient:
        return gradient().pattern_();
    default:
        error::raise(error::NotImplemented);
        break;
    }
    return nullptr;
}

brush & brush::operator=(const class color &c) {
    set_color(c);
    return *this;
}

brush & brush::operator=(const color::name &name) {
    set_color(name);
    return *this;
}

brush & brush::operator=(const class gradient &gradient) {
    set_gradient(gradient);
    return *this;
}

brush & brush::operator=(const brush &br) {
    switch (br.type()){
    case type::Null:
        type_ = type::Null;
        d->color.reset(nullptr);
        d->gradient.reset(nullptr);
        break;

    case type::Solid:
        set_color(br.color());
        break;

    case type::Gradient:
        set_gradient(br.gradient());
        break;

    case type::Surface:
        error::raise(error::NotImplemented); // TODO not implement yet
        break;
    }
    return *this;
}

brush & brush::operator=(brush &&brush) {
    this->type_ = brush.type_;
    this->d = std::move(brush.d);
    return *this;
}

color & brush::color() {
    if (type() != type::Solid) {
        error::raise(error::BrushTypeMismatch);
    }
    return *d->color;
}

const color & brush::color() const {
    return const_cast<brush *>(this)->color();
}

void brush::set_color(const class color &c) {
    if (type() == type::Gradient) {
        d->gradient.reset(nullptr);
    }
    type_ = type::Solid;
    d->color.reset(new (class color)(c));
}

class gradient & brush::gradient() {
    if (type() != type::Gradient) {
        error::raise(error::BrushTypeMismatch);
    }
    return *d->gradient;
}

const class gradient & brush::gradient() const {
    return const_cast<brush *>(this)->gradient();
}

void brush::set_gradient(const class gradient& gradient) {
    type_ = type::Gradient;
    switch (gradient.type()) {
    case gradient::type::Linear:
        d->gradient.reset(new linear_gradient(reinterpret_cast<const linear_gradient &>(gradient)));
        break;
    case gradient::type::Radial:
        d->gradient.reset(new radial_gradient(reinterpret_cast<const radial_gradient &>(gradient)));
        break;
    }
    d->color.reset(nullptr);
}

std::string brush::to_string() const {
    switch (type()) {
    case type::Solid:
        return color().to_string();

    case type::Gradient:
        return gradient().to_string();

    case type::Surface:
        return "Surface";
        
    case type::Null:
        return "Null";
    }
}

std::ostream & operator<<(std::ostream &out, const brush &brush) {
    return out << brush.to_string();
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
