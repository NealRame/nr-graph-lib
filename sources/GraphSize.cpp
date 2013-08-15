/*
 * GraphSize.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: jux
 */

#include <algorithm>
#include <cmath>

#include <boost/format.hpp>

#include "GraphSize.h"

namespace com {
namespace nealrame {
namespace graph {

Size Size::boundedTo(const Size & otherSize) const {
	return Size(fmin(_width, otherSize._width), fmin(_height, otherSize._height));
}

Size Size::expandedTo(const Size & otherSize) const {
	return Size(fmax(_width, otherSize._width), fmax(_height, otherSize._height));
}

bool Size::isValid() const {
	return _width >=0 && _height >= 0;
}

std::string Size::toString() const {
	return (boost::format("%1%x%2%") % _width % _height).str();
}

std::ostream & operator<<(std::ostream &out, const Size &size) {
	return out << size.toString();
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
