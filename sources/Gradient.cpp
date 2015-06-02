/*
 * GraphGradient.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: jux
 */

#include <NRGraph/Gradient.h>
#include <NRGraph/Error.h>

#include <algorithm>
#include <sstream>

#include <boost/format.hpp>

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

void Gradient::addColorStop(double offset, const color &c) {
    addColorStop({offset, c});
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
    return "Gradient";
}

std::string Gradient::toString() const {
    std::ostringstream ss;
    ss << typeToString() << "(";
    for (int i = 0, count = colorStopCount(); i < count; ++i) {
        const Stop &stop(_stops.at(i));
        ss << (boost::format("%1% %2%%3%")
            % stop.color.toString()
            % stop.offset
            % ((i < count - 1) ? ", ":")")).str();
    }
    return ss.str();
}

bool operator==(const Gradient::Stop &ls, const Gradient::Stop & rs) {
    return ls.offset == rs.offset && ls.color == rs.color;
}

std::ostream & operator<<(std::ostream &out, const Gradient &gradient) {
    return out << gradient.toString();
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
