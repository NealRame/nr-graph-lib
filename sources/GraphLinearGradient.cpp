/*
 * GraphLinearGradient.cpp
 *
 *  Created on: 13 juin 2013
 *      Author: jux
 */

#include <vector>
#include <string>

#include "GraphColor.h"
#include "GraphPoint.h"
#include "GraphLinearGradient.h"

namespace com {
namespace nealrame {
namespace graph {

LinearGradient::LinearGradient(const Point &start, const Point &end) :
	_start_point(start),
	_end_point(end) {
}

LinearGradient::LinearGradient(const LinearGradient &other) :
	Gradient(other),
	_start_point(other._start_point),
	_end_point(other._end_point) {
}

LinearGradient::~LinearGradient() {
}

LinearGradient & LinearGradient::operator=(const Gradient &gradient) {
	Gradient::operator=(gradient);
	return *this;
}

std::string LinearGradient::typeToString() const {
	return "LineraGradient";
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
