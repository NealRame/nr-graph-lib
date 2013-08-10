/*
 * GraphGradient.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: jux
 */

#include <algorithm>
#include <sstream>

#include "GraphError.h"
#include "GraphGradient.h"

namespace com {
namespace nealrame {
namespace graph {

Gradient::Gradient() :
	_extend(Gradient::Extend::Pad) {
}

Gradient::Gradient(const Gradient &gradient) :
	_extend(gradient._extend),
	_stops(gradient._stops) {
}

Gradient::~Gradient() {
}

void Gradient::addColorStop(const Stop &stop){
	auto it = std::find_if(_stops.begin(), _stops.end(),
							[stop](Stop s){ return stop.offset > s.offset; });
	_stops.insert(it, stop);
}

void Gradient::addColorStop(double offset, const Color &color) {
	addColorStop({offset, color});
}

void Gradient::addColorStops(const std::vector<Stop> &stops) {
	for (const Stop &s : stops) {
		addColorStop(s);
	}
}

void Gradient::setColorStops(const std::vector<Stop> &stops) {
	_stops.clear();
	addColorStops(stops);
}

Gradient::Stop Gradient::colorStop(unsigned int index) const {
	if (index < _stops.size()) {
		return _stops.at(index);
	}
	throw Error(Error::InvalidColorStopIndex);
}

Gradient & Gradient::operator=(const Gradient &other) {
	setExtend(other.extend());
	setColorStops(colorStops());
	return *this;
}

bool Gradient::operator==(const Gradient &other) {
	return _extend == other._extend && _stops == other._stops;
}

std::string Gradient::typeToString() const {
	return "NoGradient";
}

std::string Gradient::toString() const {
	std::ostringstream ss;
	ss << typeToString() << "::";
	for (int i = 0, count = colorStopCount(); i < count; ++i) {
		const Stop &stop(_stops.at(i));
		ss << stop.offset << ',' << stop.color.converTo(Color::Rgb).toString() << ((i < count - 1) ? ':':';');
	}
	return ss.str();
}

bool operator==(const Gradient::Stop &ls, const Gradient::Stop & rs) {
	return ls.offset == rs.offset && ls.color == rs.color;
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
