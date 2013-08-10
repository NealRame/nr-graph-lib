/*
 * GraphPen.h
 *
 *  Created on: Jun 12, 2013
 *      Author: jux
 */

#ifndef GRAPHPEN_H_
#define GRAPHPEN_H_

namespace com {
namespace nealrame {
namespace graph {

class Pen {
public:
	Pen();
	virtual ~Pen();

public:
	double width() const        { return _width;  }
	void setWidth(double width) { _width = width; }

private:
	double _width;
};

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
#endif /* GRAPHPEN_H_ */
