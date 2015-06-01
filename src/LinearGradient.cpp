/*
 * GraphLinearGradient.cpp
 *
 *  Created on: 13 juin 2013
 *      Author: jux
 */

#include <NRGraph/LinearGradient.h>
#include <NRGraph/Color.h>
#include <NRGraph/Point.h>

#include <vector>
#include <string>

using namespace com::nealrame::graph;

LinearGradient::LinearGradient() {
}

LinearGradient::LinearGradient(const Point &start, const Point &end)
    : _start_point(start)
    , _end_point(end) {
}

LinearGradient::LinearGradient(const LinearGradient &other)
    : Gradient(other)
    , _start_point(other._start_point)
    , _end_point(other._end_point) {
}

LinearGradient::~LinearGradient() {
}

LinearGradient & LinearGradient::operator=(const Gradient &gradient) {
    Gradient::operator=(gradient);
    return *this;
}

std::string LinearGradient::typeToString() const {
    return "LinearGradient";
}
