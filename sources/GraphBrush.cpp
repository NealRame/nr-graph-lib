/*
 * Brush.cpp
 *
 *  Created on: 12 juin 2013
 *      Author: jux
 */

extern "C" {
#	include <cairo.h>
}

#include "GraphError.h"
#include "GraphLinearGradient.h"
#include "GraphRadialGradient.h"
#include "GraphBrush.h"

namespace com {
namespace nealrame {
namespace graph {

#define CAIRO_PATTERN(PTR) reinterpret_cast<cairo_pattern_t  *>(PTR)

Brush::Brush() :
	_type(Type::Null) {
}

Brush::Brush(const Color &color) :
	Brush() {
	setColor(color);
}

Brush::Brush(const Color::Name name) :
	Brush() {
	setColor(name);
}

Brush::Brush(const graph::Gradient &gradient) :
	Brush() {
	setGradient(gradient);
}

Brush::Brush(const Brush &brush) {
	*this = brush;
}

Brush::~Brush() {
	if (type() == Type::Gradient) {
		delete _gradient;
	}
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
		_type = Type::Null;
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

Brush::Type Brush::type() const {
	return _type;
}

Color &  Brush::color() {
	if (type() != Type::Solid) {
		Error::raise(Error::BrushTypeMismatch);
	}
	return _color;
}

const Color & Brush::color() const {
	return const_cast<const Color &>(const_cast<Brush *>(this)->color());
}

void Brush::setColor(const Color &color) {
	if (type() == Type::Gradient) {
		delete _gradient;
	}
	_type = Type::Solid;
	_color = color;
}

Gradient & Brush::gradient() {
	if (type() != Type::Gradient) {
		Error::raise(Error::BrushTypeMismatch);
	}
	return *_gradient;
}

const Gradient & Brush::gradient() const {
	return const_cast<const Gradient &>(const_cast<Brush *>(this)->gradient());
}

void Brush::setGradient(const graph::Gradient& gradient) {
	if (type() == Type::Gradient) {
		delete _gradient;
	}
	_type = Type::Gradient;
	switch (gradient.type()) {
	case Gradient::Type::Linear:
		_gradient = new LinearGradient(reinterpret_cast<const LinearGradient &>(gradient));
		break;
	case Gradient::Type::Radial:
		_gradient =  new RadialGradient(reinterpret_cast<const RadialGradient &>(gradient));
		break;
	}
}

void * Brush::exportToCairoPattern() const {
	cairo_pattern_t *pat = NULL;

	switch (type()) {
	case Type::Null:
		pat = NULL;
		break;

	case Type::Solid:
		{
			Color::RGB rgb = _color.rgb();
			pat = cairo_pattern_create_rgba(rgb.red, rgb.green, rgb.blue, _color.alpha());
		}
		break;

	case Type::Gradient:
		{
			switch (_gradient->type()) {
			case Gradient::Type::Linear:
				{
					LinearGradient &linear = *(reinterpret_cast<LinearGradient *>(_gradient));
					Point p1 = linear.startPoint(), p2 = linear.endPoint();
					pat = cairo_pattern_create_linear(p1.x(), p1.y(), p2.x(), p2.y());
				}
				break;
			case Gradient::Type::Radial:
				{
					RadialGradient &radial  = *(reinterpret_cast<RadialGradient *>(_gradient));
					Point c1 = radial.startCircleCenterPoint(), c2 = radial.endCircleCenterPoint();
					double r1 = radial.startCircleRadius(), r2 = radial.endCircleRadius();
					pat = cairo_pattern_create_radial(c1.x(), c1.y(), r1, c2.x(), c2.y(), r2);
				}
				break;
			}

			switch (_gradient->extend()) {
			case Gradient::Extend::None:
				cairo_pattern_set_extend(pat, CAIRO_EXTEND_NONE);
				break;
			case Gradient::Extend::Pad:
				cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
				break;
			case Gradient::Extend::Reflect:
				cairo_pattern_set_extend(pat, CAIRO_EXTEND_REFLECT);
				break;
			case Gradient::Extend::Repeat:
				cairo_pattern_set_extend(pat, CAIRO_EXTEND_REPEAT);
				break;
			}


			for (const Gradient::Stop &stop : _gradient->colorStops()) {
				Color::RGB rgb = stop.color.rgb();
				cairo_pattern_add_color_stop_rgba(pat, stop.offset, rgb.red, rgb.green, rgb.blue, stop.color.alpha());
			}
		}
		break;

	case Type::Surface:
		Error::raise(Error::NotImplemented); // TODO not implement yet
		break;
	}

	return pat;
}

void Brush::importFromCairoPattern(void *ptr) {
	cairo_pattern_t *pat = reinterpret_cast<cairo_pattern_t *>(ptr);
	cairo_pattern_type_t pattern_type = cairo_pattern_get_type(pat);

	if (type() == Type::Gradient) {
		delete _gradient;
	}

	switch(pattern_type) {
	case CAIRO_PATTERN_TYPE_SOLID:
		{
			_type = Type::Solid;

			double r, g, b, a;
			cairo_pattern_get_rgba(pat, &r, &g, &b, &a);

			_color = Color(Color::RGB{r, g, b}, a);
		}
		break;

	case CAIRO_PATTERN_TYPE_LINEAR:
	case CAIRO_PATTERN_TYPE_RADIAL:
		{
			_type = Type::Gradient;

			if (pattern_type == CAIRO_PATTERN_TYPE_LINEAR) {
				Point p1, p2;
				cairo_pattern_get_linear_points(pat, &(p1.x()), &(p1.y()), &(p2.x()), &(p2.y()));
				_gradient = new LinearGradient(p1, p2);
			} else {
				Point p1, p2;
				double r1, r2;
				cairo_pattern_get_radial_circles(pat, &(p1.x()), &(p1.y()), &r1, &(p2.x()), &(p2.y()), &r2);
				_gradient = new RadialGradient(p1, r1, p2, r2);
			}

			switch(cairo_pattern_get_extend(pat)) {
			case CAIRO_EXTEND_NONE:
				_gradient->setExtend(Gradient::Extend::None);
				break;

			case CAIRO_EXTEND_PAD:
				_gradient->setExtend(Gradient::Extend::Pad);
				break;

			case CAIRO_EXTEND_REFLECT:
				_gradient->setExtend(Gradient::Extend::Reflect);
				break;

			case CAIRO_EXTEND_REPEAT:
				_gradient->setExtend(Gradient::Extend::Repeat);
				break;
			}

			int stop_count = 0;

			cairo_pattern_get_color_stop_count(pat, &stop_count);
			for (int i = 0; i < stop_count; ++i) {
				double offset, r, g, b, a;
				cairo_pattern_get_color_stop_rgba(pat, i, &offset, &r, &g, &b, &a);
				_gradient->addColorStop(Gradient::Stop{offset, Color(Color::RGB{r, g, b}, a)});
			}
		}
		break;

	case CAIRO_PATTERN_TYPE_SURFACE:
	case CAIRO_PATTERN_TYPE_MESH:
	case CAIRO_PATTERN_TYPE_RASTER_SOURCE:
		Error::raise(Error::NotImplemented); // TODO not implement yet
		break;
	}
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
