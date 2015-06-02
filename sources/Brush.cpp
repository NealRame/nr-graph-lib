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
    std::unique_ptr<Color> color;
    std::unique_ptr<Gradient> gradient;
};

brush::brush()
    : type_(type::Null)
    , d(new impl) {
}

brush::brush(const Color &color)
    : brush() {
    set_color(color);
}

brush::brush(const Color::Name name)
    : brush() {
    set_color(name);
}

brush::brush(const graph::Gradient &gradient)
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
        set_color(Color(ptr));
        break;

    case CAIRO_PATTERN_TYPE_LINEAR:
        set_gradient(LinearGradient(ptr));
        break;

    case CAIRO_PATTERN_TYPE_RADIAL:
        set_gradient(RadialGradient(ptr));
        break;
        
    case CAIRO_PATTERN_TYPE_SURFACE:
    case CAIRO_PATTERN_TYPE_MESH:
    case CAIRO_PATTERN_TYPE_RASTER_SOURCE:
        Error::raise(Error::NotImplemented); // TODO not implement yet
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
        Error::raise(Error::NotImplemented);
        break;
    }
    return nullptr;
}

brush & brush::operator=(const Color &color) {
    set_color(color);
    return *this;
}

brush & brush::operator=(const Color::Name &name) {
    set_color(name);
    return *this;
}

brush & brush::operator=(const Gradient &gradient) {
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
        Error::raise(Error::NotImplemented); // TODO not implement yet
        break;
    }
    return *this;
}

brush & brush::operator=(brush &&brush) {
    this->type_ = brush.type_;
    this->d = std::move(brush.d);
    return *this;
}

Color & brush::color() {
    if (type() != type::Solid) {
        Error::raise(Error::BrushTypeMismatch);
    }
    return *d->color;
}

const Color & brush::color() const {
    return const_cast<brush *>(this)->color();
}

void brush::set_color(const Color &color) {
    if (type() == type::Gradient) {
        d->gradient.reset(nullptr);
    }
    type_ = type::Solid;
    d->color.reset(new Color(color));
}

Gradient & brush::gradient() {
    if (type() != type::Gradient) {
        Error::raise(Error::BrushTypeMismatch);
    }
    return *d->gradient;
}

const Gradient & brush::gradient() const {
    return const_cast<brush *>(this)->gradient();
}

void brush::set_gradient(const graph::Gradient& gradient) {
    type_ = type::Gradient;
    switch (gradient.type()) {
    case Gradient::Type::Linear:
        d->gradient.reset(new LinearGradient(reinterpret_cast<const LinearGradient &>(gradient)));
        break;
    case Gradient::Type::Radial:
        d->gradient.reset(new RadialGradient(reinterpret_cast<const RadialGradient &>(gradient)));
        break;
    }
    d->color.reset(nullptr);
}

std::string brush::to_string() const {
    switch (type()) {
    case type::Solid:
        return color().toString();

    case type::Gradient:
        return gradient().toString();

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
