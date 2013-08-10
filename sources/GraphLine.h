/*
 * Line.h
 *
 *  Created on: Jun 9, 2013
 *      Author: jux
 */

#ifndef LINE_H_
#define LINE_H_

#include "GraphPoint.h"

namespace com {
namespace nealrame {
namespace graph {

class Point;
class Line {
public:
	Line();
	Line(const Point &p1, const Point &p2);
	Line(double x1, double y1, double x2, double y2);
	virtual ~Line();

public:
	bool isNull() const { return _p1 == _p2; }

public:
	Point & p1()             { return _p1; }
	const Point & p1() const { return _p1; }
	Point & p2()             { return _p2; }
	const Point & p2() const { return _p2; }

	double dx() const;
	double dy() const;

	void translate(const Point &offset);
	void translate(double dx, double dy);
	Line translated(const Point &offset) const;
	Line translated(double dx, double dy) const;

public:
	bool operator==(const Line &line) const {
		return this->_p1 == line._p1 && this->_p2 == line._p2;
	}
	bool operator!=(const Line &line) const {
		return ! (*this == line);
	}

private:
	Point _p1;
	Point _p2;
};

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
#endif /* LINE_H_ */
