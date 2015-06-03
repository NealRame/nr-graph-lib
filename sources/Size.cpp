/*
 * GraphSize.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: jux
 */

#include <NRGraph/Size.h>

#include <algorithm>
#include <cmath>

#include <boost/format.hpp>

namespace com {
namespace nealrame {
namespace graph {

size size::bounded_to(const size & otherSize) const {
    return size(fmin(_width, otherSize._width), fmin(_height, otherSize._height));
}

size size::expanded_to(const size & otherSize) const {
    return size(fmax(_width, otherSize._width), fmax(_height, otherSize._height));
}

bool size::is_valid() const {
    return _width >=0 && _height >= 0;
}

std::string size::to_string() const {
    return (boost::format("%1%x%2%") % _width % _height).str();
}

std::ostream & operator<<(std::ostream &out, const size &size) {
    return out << size.to_string();
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
