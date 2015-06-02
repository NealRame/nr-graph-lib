/*
 * Brush.cpp
 *
 *  Created on: 12 juin 2013
 *      Author: jux
 */

#include <NRGraph/Brush.h>

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

Brush::Brush()
    : type_(Type::Null) {
}

Brush::Brush(const Color &color)
    : Brush() {
    setColor(color);
}

Brush::Brush(const Color::Name name)
    : Brush() {
    setColor(name);
}

Brush::Brush(const graph::Gradient &gradient)
    : Brush() {
    setGradient(gradient);
}

Brush::Brush(const Brush &brush)
    : Brush() {
    *this = brush;
}

Brush::Brush(const void *ptr) {
    auto pattern = reinterpret_cast<cairo_pattern_t *>((void *)ptr);

    switch (::cairo_pattern_get_type(pattern)) {
    case CAIRO_PATTERN_TYPE_SOLID:
        setColor(Color(ptr));
        break;

    case CAIRO_PATTERN_TYPE_LINEAR:
        setGradient(LinearGradient(ptr));
        break;

    case CAIRO_PATTERN_TYPE_RADIAL:
        setGradient(RadialGradient(ptr));
        break;
        
    case CAIRO_PATTERN_TYPE_SURFACE:
    case CAIRO_PATTERN_TYPE_MESH:
    case CAIRO_PATTERN_TYPE_RASTER_SOURCE:
        Error::raise(Error::NotImplemented); // TODO not implement yet
        break;
    }
}

Brush::~Brush() {
    if (type() == Type::Gradient) {
        delete gradient_;
    }
}

std::shared_ptr<void> Brush::pattern_() const {
    switch (type()) {
    case Type::Null:
        return nullptr;
    case Type::Solid:
        return color().pattern_();
    case Type::Gradient:
        return gradient().pattern_();
    default:
        Error::raise(Error::NotImplemented);
        break;
    }
    return nullptr;
}

Brush & Brush::operator=(const Color &color) {
    setColor(color);
    return *this;
}

Brush & Brush::operator=(const Color::Name &name) {
    setColor(name);
    return *this;
}

Brush & Brush::operator=(const Gradient &gradient) {
    setGradient(gradient);
    return *this;
}

Brush & Brush::operator=(const Brush &brush) {
    switch (brush.type()){
    case Type::Null:
        type_ = Type::Null;
        break;

    case Type::Solid:
        setColor(brush.color());
        break;

    case Type::Gradient:
        setGradient(brush.gradient());
        break;

    case Type::Surface:
        Error::raise(Error::NotImplemented); // TODO not implement yet
        break;
    }
    return *this;
}

Color &  Brush::color() {
    if (type() != Type::Solid) {
        Error::raise(Error::BrushTypeMismatch);
    }
    return color_;
}

const Color & Brush::color() const {
    return const_cast<const Color &>(const_cast<Brush *>(this)->color());
}

void Brush::setColor(const Color &color) {
    if (type() == Type::Gradient) {
        delete gradient_;
    }
    type_ = Type::Solid;
    color_ = color;
}

Gradient & Brush::gradient() {
    if (type() != Type::Gradient) {
        Error::raise(Error::BrushTypeMismatch);
    }
    return *gradient_;
}

const Gradient & Brush::gradient() const {
    return const_cast<const Gradient &>(const_cast<Brush *>(this)->gradient());
}

void Brush::setGradient(const graph::Gradient& gradient) {
    if (type() == Type::Gradient) {
        delete gradient_;
    }
    type_ = Type::Gradient;
    switch (gradient.type()) {
    case Gradient::Type::Linear:
        gradient_ = new LinearGradient(reinterpret_cast<const LinearGradient &>(gradient));
        break;
    case Gradient::Type::Radial:
        gradient_ =  new RadialGradient(reinterpret_cast<const RadialGradient &>(gradient));
        break;
    }
}

std::string Brush::toString() const {
    switch (type()) {
    case Brush::Type::Solid:
        return color().toString();

    case Brush::Type::Gradient:
        return gradient().toString();

    case Brush::Type::Surface:
        return "Surface";
        
    case Brush::Type::Null:
        return "Null";
    }
}

std::ostream & operator<<(std::ostream &out, const Brush &brush) {
    return out << brush.toString();
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
