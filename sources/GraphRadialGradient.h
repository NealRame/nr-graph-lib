/*
 * GraphRadialGradient.h
 *
 *  Created on: 13 juin 2013
 *      Author: jux
 */

#ifndef GRAPHRADIALGRADIENT_H_
#define GRAPHRADIALGRADIENT_H_

#include "GraphGradient.h"

namespace com {
namespace nealrame {
namespace graph {
class Point;
class RadialGradient : public Gradient {
public:
	RadialGradient(const Point &center, double radius);
	RadialGradient(const Point &starCenter, double startRadius, const Point &endCenter, double endRadius);
	RadialGradient(const RadialGradient &);
	virtual ~RadialGradient();

public:
	virtual Type type() const { return Gradient::Type::Radial; }

public:
	Point startCircleCenterPoint() const
	{ return _start_circle_center; }
	void setStartCircleCenterPoint(const Point &p)
	{ _start_circle_center = p; }

	double startCircleRadius() const
	{ return _start_circle_radius; }
	void setStartCircleRadius(double radius)
	{ _start_circle_radius = radius; }

	Point endCircleCenterPoint() const
	{ return _end_circle_center; }
	void setEndCircleCenterPoint(const Point &p)
	{ _end_circle_center = p; }

	double endCircleRadius() const
	{ return _end_circle_radius; }
	void setEndCircleRadius(double radius)
	{ _end_circle_radius = radius; }

public:
	RadialGradient & operator=(const Gradient &);

public:
	virtual std::string typeToString() const;

private:
	Point _start_circle_center;
	double _start_circle_radius;
	Point _end_circle_center;
	double _end_circle_radius;
};

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
#endif /* GRAPHRADIALGRADIENT_H_ */
