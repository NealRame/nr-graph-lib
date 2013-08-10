/*
 * GraphGradient.h
 *
 *  Created on: Jun 12, 2013
 *      Author: jux
 */

#ifndef GRAPHGRADIENT_H_
#define GRAPHGRADIENT_H_

#include <string>
#include <vector>

#include "GraphColor.h"

namespace com {
namespace nealrame {
namespace graph {
class Gradient {
public:
	enum class Type {
		Linear,
		Radial
	};

	enum class Extend {
		None,
		Pad,
		Repeat,
		Reflect
	};

	struct Stop {
		double offset;
		Color color;
	};

public:
	Gradient();
	Gradient(const Gradient &);
	virtual ~Gradient();

public:
	virtual Type type() const = 0;
	virtual Extend extend() const         { return _extend; }
	virtual void setExtend(Extend extend) { _extend = extend; }

public:
	virtual void addColorStop(double, const Color &);
	virtual void addColorStop(const Stop &);
	virtual void addColorStops(const std::vector<Stop> &);
	virtual void setColorStops(const std::vector<Stop> &);

	virtual Stop colorStop(unsigned int index) const;
	virtual unsigned int colorStopCount() const  { return _stops.size(); }
	virtual std::vector<Stop> colorStops() const { return _stops; }

public:
	virtual Gradient & operator=(const Gradient &);
	virtual bool operator==(const Gradient &);

public:
	virtual std::string typeToString() const;
	virtual std::string toString() const;

protected:
	Extend _extend;
	std::vector<Stop> _stops;
};

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
#endif /* GRAPHGRADIENT_H_ */
