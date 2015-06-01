/*
 * GraphRadialGradient.cpp
 *
 *  Created on: 13 juin 2013
 *      Author: jux
 */

#include <NRGraph/RadialGradient.h>
#include <NRGraph/Color.h>
#include <NRGraph/Point.h>

#include <vector>
#include <string>

using namespace com::nealrame::graph;

RadialGradient::RadialGradient(const Point &center, double radius)
    : _start_circle_center(center)
    , _start_circle_radius(radius)
    , _end_circle_center(center)
    , _end_circle_radius(radius) {
}

RadialGradient::RadialGradient(const Point &startCenter, double startRadius, const Point &endCenter, double endRadius)
    : _start_circle_center(startCenter)
    , _start_circle_radius(startRadius)
    , _end_circle_center(endCenter)
    , _end_circle_radius(endRadius) {
}

RadialGradient::RadialGradient(const RadialGradient &other)
    : Gradient(other)
    , _start_circle_center(other.startCircleCenterPoint())
    , _start_circle_radius(other.startCircleRadius())
    , _end_circle_center(other.endCircleCenterPoint())
    , _end_circle_radius(other.endCircleRadius()) {
}

RadialGradient::~RadialGradient() {
}

RadialGradient & RadialGradient::operator=(const Gradient &gradient) {
    Gradient::operator =(gradient);
    return *this;
}

std::string RadialGradient::typeToString() const {
    return "RadialGradient";
}
