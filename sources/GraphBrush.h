/*
 * Brush.h
 *
 *  Created on: 12 juin 2013
 *      Author: jux
 */

#ifndef BRUSH_H_
#define BRUSH_H_

#include "GraphColor.h"
#include "GraphGradient.h"

namespace com {
namespace nealrame {
namespace graph {
class Brush {
	friend class Painter;

public:
	enum class Type {
		Gradient,
		Solid,
		Surface,
		Null,
	};

public:
	Brush();
	Brush(const Color &);
	Brush(const Color::Name);
	Brush(const Gradient &);
	Brush(const Brush &);
	virtual ~Brush();

public:
	Brush & operator=(const Color &);
	Brush & operator=(const Color::Name &);
	Brush & operator=(const Gradient &);
	Brush & operator=(const Brush &);

public:
	bool isNull() const { return type() == Type::Null; }

public:
	Type type() const;

public:
	Color & color();
	const Color & color() const;
	void setColor(const Color &);

	Gradient & gradient();
	const Gradient & gradient() const;
	void setGradient(const Gradient &);

private:
	void * exportToCairoPattern() const;
	void importFromCairoPattern(void *);

private:
	Type _type;
	union {
		Color _color;
		Gradient *_gradient;
	};
};

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
#endif /* BRUSH_H_ */