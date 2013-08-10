/*
 * GraphSurface.h
 *
 *  Created on: 11 juin 2013
 *      Author: jux
 */

#ifndef GRAPHSURFACE_H_
#define GRAPHSURFACE_H_

#include <string>

#include "GraphSize.h"

namespace com {
namespace nealrame {
namespace graph {
class Painter;
class Surface {
	friend class Painter;
public:
	enum class Backend {
		Image,
		Pdf,
		Svg,
		Window
	};

public:
	Surface(Size, Backend backend);
	virtual ~Surface();

public:
	Size size() const;

public:
	void exportToPNG(const std::string &) const;

private:
	void *_priv_data;
};

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
#endif /* GRAPHSURFACE_H_ */
