/*
 * GraphLinearGradient.h
 *
 *  Created on: 13 juin 2013
 *      Author: jux
 */

#ifndef GRAPHLINEARGRADIENT_H_
#define GRAPHLINEARGRADIENT_H_

#include "GraphPoint.h"
#include "GraphGradient.h"

namespace com {
namespace nealrame {
namespace graph {
class LinearGradient : public Gradient {
public:
	LinearGradient(const Point &start, const Point &end);
	LinearGradient(const LinearGradient &other);
	virtual ~LinearGradient();

public:
	virtual Type type() const              { return Gradient::Type::Linear; }

public:
	Point startPoint() const               { return _start_point; }
	void setStartPoint(const Point &point) { _start_point = point; }
	Point endPoint() const                 { return _end_point; }
	void setEndPoint(const Point &point)   { _end_point = point; }

public:
	LinearGradient & operator=(const Gradient &);

public:
	virtual std::string typeToString() const;

private:
	Point _start_point;
	Point _end_point;
};

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
#endif /* GRAPHLINEARGRADIENT_H_ */
