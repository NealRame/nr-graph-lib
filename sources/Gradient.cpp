/*
 * Graphgradient.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: jux
 */

#include <NRGraph/gradient.h>
#include <NRGraph/error.h>

#include <algorithm>
#include <sstream>

#include <boost/format.hpp>

namespace com {
namespace nealrame {
namespace graph {

gradient::gradient() :
    _extend(gradient::extend::Pad) {
}

gradient::gradient(const gradient &gradient) :
    _extend(gradient._extend),
    _stops(gradient._stops) {
}

gradient::~gradient() {
}

void gradient::add_color_stop(const stop &stop){
    auto it = std::find_if(
        _stops.begin(), _stops.end(),
        [stop](gradient::stop s) {
            return stop.offset > s.offset;
        }
    );
    _stops.insert(it, stop);
}

void gradient::add_color_stop(double offset, const color &c) {
    add_color_stop({offset, c});
}

void gradient::add_color_stops(const std::vector<stop> &stops) {
    for (const auto &s : stops) {
        add_color_stop(s);
    }
}

void gradient::set_color_stops(const std::vector<stop> &stops) {
    _stops.clear();
    add_color_stops(stops);
}

gradient::stop gradient::color_stop(unsigned int index) const {
    if (index < _stops.size()) {
        return _stops.at(index);
    }
    throw error(error::InvalidColorStopIndex);
}

gradient & gradient::operator=(const gradient &other) {
    set_extend(other.extend());
    set_color_stops(color_stops());
    return *this;
}

bool gradient::operator==(const gradient &other) {
    return _extend == other._extend && _stops == other._stops;
}

std::string gradient::type_to_string() const {
    return "gradient";
}

std::string gradient::to_string() const {
    std::ostringstream ss;
    ss << type_to_string() << "(";
    for (auto i = 0u, count = color_stop_count(); i < count; ++i) {
        const stop &stop(_stops.at(i));
        ss << (boost::format("%1% %2%%3%")
            % stop.color.to_string()
            % stop.offset
            % ((i < count - 1) ? ", ":")")).str();
    }
    return ss.str();
}

bool operator==(const gradient::stop &ls, const gradient::stop & rs) {
    return ls.offset == rs.offset && ls.color == rs.color;
}

std::ostream & operator<<(std::ostream &out, const gradient &gradient) {
    return out << gradient.to_string();
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
