/*
 * Color.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: jux
 */

#include <cmath>
#include <sstream>

#include <boost/format.hpp>

#include "GraphColor.h"
#include "GraphError.h"

namespace com {
namespace nealrame {
namespace graph {

template<typename T> inline T max(T a, T b) {
	return a < b ? b : a;
}

template<typename T> inline T max(T a, T b, T c) {
	return max<T>(max<T>(a, b), c);
}

template<typename T> inline T min(T a, T b) {
	return a < b ? a : b;
}

template<typename T> inline T min(T a, T b, T c) {
	return min<T>(min<T>(a, b), c);
}

template<typename MODEL> bool isValid(const MODEL &m);

template<> bool isValid<Color::CMYK>(const Color::CMYK &cmyk) {
	return cmyk.cyan >= 0 && cmyk.cyan <= 1
		&& cmyk.magenta >= 0 && cmyk.magenta <= 1
		&& cmyk.yellow >= 0 && cmyk.yellow <= 1
		&& cmyk.black >= 0 && cmyk.black <= 1;
}

template<> bool isValid<Color::HSL>(const Color::HSL &hsl) {
	return hsl.hue >= 0 && hsl.hue < 360
		&& hsl.saturation >= 0 && hsl.saturation <= 1
		&& hsl.lightness >= 0 && hsl.lightness <= 1;
}

template<> bool isValid<Color::HSV>(const Color::HSV &hsv) {
	return hsv.hue >= 0 && hsv.hue < 360
		&& hsv.saturation >= 0 && hsv.saturation <= 1
		&& hsv.value >= 0 && hsv.value <= 1;
}

template<> bool isValid<Color::RGB>(const Color::RGB &rgb) {
	return rgb.red >= 0 && rgb.red <= 1
		&& rgb.green >= 0 && rgb.green <= 1
		&& rgb.blue >= 0 && rgb.blue <= 1;
}

template<typename SOURCE>
SOURCE convert(const SOURCE &source) {
	return source;
}

template<typename SOURCE, typename TARGET>
TARGET convert(const SOURCE &source) {
	if (! isValid<SOURCE>(source)) {
		Error::raise(Error::InvalidValue);
	}
	return convert<Color::RGB, TARGET>(convert<SOURCE, Color::RGB>(source));
}

template<>
Color::CMYK convert(const Color::RGB &source) {
	if (! isValid(source)) {
		Error::raise(Error::InvalidValue);
	}

	Color::CMYK cmyk;

	double k = 1 - max(source.red, source.green, source.blue);

	cmyk.cyan = (1 - source.red - k)/(1 - k);
	cmyk.magenta = (1 - source.green - k)/(1 - k);
	cmyk.yellow = (1 - source.blue - k)/(1 - k);
	cmyk.black = k;

	return cmyk;
	}

template<>
Color::RGB convert(const Color::CMYK &source) {
	if (! isValid(source)) {
		Error::raise(Error::InvalidValue);
	}

	Color::RGB rgb;

	double k = 1 - source.black;

	rgb.red   = k*(1 - source.cyan);
	rgb.green = k*(1 - source.magenta);
	rgb.blue =  k*(1 - source.yellow);

	return rgb;
}

template<>
Color::HSL convert(const Color::RGB &source) {
	if (! isValid(source)) {
		Error::raise(Error::InvalidValue);
	}

	double cmax  = max(source.red, source.green, source.blue);
	double cmin  = min(source.red, source.green, source.blue);
	double delta = cmax - cmin;

	Color::HSL hsl;

	hsl.lightness = (cmax + cmin)/2;

	if (delta != 0) {
		if (cmax == source.red) {
			hsl.hue = std::fmod((source.green - source.blue)/delta, 6.);
		} else
		if (cmax == source.green) {
			hsl.hue = ((source.blue - source.red)/delta)  + 2;
		} else
		if (cmax == source.blue) {
			hsl.hue = ((source.red - source.green)/delta) + 4;
		}
		hsl.hue = hsl.hue * 60;
		hsl.saturation = delta/(1 - std::fabs(2*hsl.lightness - 1));
	} else {
		hsl.hue = hsl.saturation = 0;
	}

	return hsl;
}

template<>
Color::RGB convert(const Color::HSL &source) {
	if (! isValid(source)) {
		Error::raise(Error::InvalidValue);
	}

	double c = source.saturation*(1 - std::fabs(2*source.lightness - 1));
	double m = source.lightness - c/2;
	double x = c*(1 - std::fabs(std::fmod(source.hue/60, 2) - 1));

	double h = source.hue;

	if (h < 60)  return { c + m, x + m, m };
	if (h < 120) return { x + m, c + m, m };
	if (h < 180) return { m, c + m, x + m };
	if (h < 240) return { m, x + m, c + m };
	if (h < 300) return { x + m, m, c + m };

	return { c + m, m, x + m };
}

template<>
Color::HSV convert(const Color::RGB &source) {
	if (! isValid(source)) {
		Error::raise(Error::InvalidValue);
	}

	double cmax  = max<double>(source.red, source.green, source.blue);
	double cmin  = min<double>(source.red, source.green, source.blue);
	double delta = cmax - cmin;

	Color::HSV hsv;

	if (delta != 0) {
		if (cmax == source.red) {
			hsv.hue = std::fmod((source.green - source.blue)/delta, 6);
		} else
		if (cmax == source.green) {
			hsv.hue = ((source.blue - source.red)/delta) + 2;
		} else
		if (cmax == source.blue) {
			hsv.hue = ((source.red  - source.green)/delta) + 4;
		}
		hsv.hue = hsv.hue * 60;

	} else {
		hsv.hue = 0;
	}

	hsv.saturation = delta/cmax;
	hsv.value = cmax;

	return hsv;
}

template<>
Color::RGB convert(const Color::HSV &source) {
	if (! isValid(source)) {
		Error::raise(Error::InvalidValue);
	}

	double c = source.value*source.saturation;
	double x = c*(1 - std::fabs(std::fmod(source.hue/60, 2) - 1));
	double m = source.value - c;

	double h = source.hue;

	if (h < 60)  return { c + m, x + m, m };
	if (h < 120) return { x + m, c + m, m };
	if (h < 180) return { m, c + m, x + m };
	if (h < 240) return { m, x + m, c + m };
	if (h < 300) return { x + m, m, c + m };

	return { c + m, m, x + m };
}

static std::string toHtmlRGBComponentString(double c) {
	std::stringstream ss;

	unsigned int v = c*255;

	if (v > 255) {
		Error::raise(Error::InvalidValue);
	}

	ss.width(2);
	ss.fill('0');
	ss << std::hex << std::noshowbase << v;

	return ss.str();
}



Color::Color() :
	_model(Invalid),
	_alpha(1) {
	_rgb.red = _rgb.green = _rgb.blue = 0;
}

Color::Color(Name name) :
	_model(Rgb),
	_alpha(1) {
	switch(name) {
	case Black:
		_rgb.red =    0; _rgb.green =    0; _rgb.blue =    0;
		break;

	case White:
		_rgb.red =    1; _rgb.green =    1; _rgb.blue =    1;
		break;

	case Red:
		_rgb.red =    1; _rgb.green =    0; _rgb.blue =    0;
		break;

	case Lime:
		_rgb.red =    0; _rgb.green =    1; _rgb.blue =    0;
		break;

	case Blue:
		_rgb.red =    0; _rgb.green =    0; _rgb.blue =    1;
		break;

	case Yellow:
		_rgb.red =    1; _rgb.green =    1; _rgb.blue =    0;
		break;

	case Cyan:
		_rgb.red =    0; _rgb.green =    1; _rgb.blue =    1;
		break;

	case Magenta:
		_rgb.red =    1; _rgb.green =    0; _rgb.blue =    1;
		break;

	case Silver:
		_rgb.red = 0.75; _rgb.green = 0.75; _rgb.blue = 0.75;
		break;

	case Gray:
		_rgb.red =  0.5; _rgb.green =  0.5; _rgb.blue =  0.5;
		break;

	case Maroon:
		_rgb.red =  0.5; _rgb.green =   0; _rgb.blue =     0;
		break;

	case Olive:
		_rgb.red =  0.5; _rgb.green =  0.5; _rgb.blue =    0;
		break;

	case Green:
		_rgb.red =    0; _rgb.green =  0.5; _rgb.blue =    0;
		break;

	case Purple:
		_rgb.red =  0.5; _rgb.green =    0; _rgb.blue =  0.5;
		break;

	case Teal:
		_rgb.red =    0; _rgb.green =  0.5; _rgb.blue =  0.5;
		break;

	case Navy:
		_rgb.red =    0; _rgb.green =    0; _rgb.blue =  0.5;
		break;
	}
}

Color::Color(const CMYK &cmyk, double alpha) :
	_model(Cmyk),
	_alpha(alpha) {
	_cmyk.cyan = cmyk.cyan;
	_cmyk.magenta = cmyk.magenta;
	_cmyk.yellow = cmyk.yellow;
	_cmyk.black = cmyk.black;
}

Color::Color(const struct RGB &rgb, double alpha) :
	_model(Rgb),
	_alpha(alpha) {
	_rgb.red = rgb.red;
	_rgb.green = rgb.green;
	_rgb.blue = rgb.blue;
}

Color::Color(const struct HSL &hsl, double alpha) :
	_model(Hsl),
	_alpha(alpha) {
	_hsl.hue = hsl.hue;
	_hsl.saturation = hsl.saturation;
	_hsl.lightness = hsl.lightness;
}

Color::Color(const struct HSV &hsv, double alpha) :
	_model(Hsv),
	_alpha(alpha) {
	_hsv.hue = hsv.hue;
	_hsv.saturation = hsv.saturation;
	_hsv.value = hsv.value;
}

Color::~Color() {
}

bool Color::isValid() const {
	return _model != Invalid;
}

bool Color::operator==(const Color &other) const {
	if (_model == other._model) {
		if (_alpha == other._alpha) {
			switch (_model) {
			case Cmyk: return false;
			case Hsl:  return false;
			case Hsv:  return false;
			case Rgb:  return false;
			default:
				break;
			}
		}
	}
	return false;
}

Color Color::converTo(Model model) const {
	switch (model) {
	case Cmyk:
		switch (_model) {
		case Cmyk: return *this;
		case Hsl:  return Color(convert<Color::HSL, Color::CMYK>(this->_hsl),  _alpha);
		case Hsv:  return Color(convert<Color::HSV, Color::CMYK>(this->_hsv),  _alpha);
		case Rgb:  return Color(convert<Color::RGB, Color::CMYK>(this->_rgb),  _alpha);
		default:
			break;
		}
		break;
	case Hsl:
		switch (_model) {
		case Hsl:  return *this;
		case Cmyk: return Color(convert<Color::CMYK, Color::HSL>(this->_cmyk), _alpha);
		case Hsv:  return Color(convert<Color::HSV, Color::HSL> (this->_hsv),  _alpha);
		case Rgb:  return Color(convert<Color::RGB, Color::HSL> (this->_rgb),  _alpha);
		default:
			break;
		}
		break;
	case Hsv:
		switch (_model) {
		case Hsv:  return *this;
		case Cmyk: return Color(convert<Color::CMYK, Color::HSV>(this->_cmyk), _alpha);
		case Hsl:  return Color(convert<Color::HSL, Color::HSV> (this->_hsl),  _alpha);
		case Rgb:  return Color(convert<Color::RGB, Color::HSV> (this->_rgb),  _alpha);
		default:
			break;
		}
		break;
	case Rgb:
		switch (_model) {
		case Rgb:  return *this;
		case Cmyk: return Color(convert<Color::CMYK, Color::RGB>(this->_cmyk), _alpha);
		case Hsl:  return Color(convert<Color::HSL, Color::RGB> (this->_hsl),  _alpha);
		case Hsv:  return Color(convert<Color::HSV, Color::RGB> (this->_hsv),  _alpha);
		default:
			break;
		}
		break;
	default:
		break;
	}
	return Color();
}

double Color::alpha() const {
	return _alpha;
}

void Color::setAlpha(double alpha) {
	_alpha = alpha;
}

Color::Model Color::model() const {
	return _model;
}

void Color::setModel(Model model) {
	*this = converTo(model);
}

Color::CMYK Color::cmyk() const {
	return _model == Cmyk ? _cmyk : converTo(Cmyk).cmyk();
}

void Color::setCmyk(const CMYK &cmyk) {
}

Color::RGB Color::rgb() const {
	return _model == Rgb  ? _rgb : converTo(Rgb).rgb();
}

void Color::setRgb(const RGB &rgb) {
	*this = Color(rgb, _alpha);
}

Color::HSL Color::hsl() const {
	return _model == Hsl ? _hsl : converTo(Hsl).hsl();
}

void Color::setHsl(const HSL &hsl) {
	*this = Color(hsl, _alpha);
}

Color::HSV Color::hsv() const {
	return _model == Hsv ? _hsv : converTo(Hsv).hsv();
}

void Color::setHsv(const HSL &hsv) {
	*this = Color(hsv, _alpha);
}

Color Color::lighter(double factor) const {
	if (factor < 0) {
		Error::raise(Error::InvalidValue);
	}
	HSV c = hsv();
	c.value = min<double>(c.value*factor, 1);
	return Color(c, _alpha).converTo(_model);
}

Color Color::darker(double factor) const {
	if (factor < 0) {
		Error::raise(Error::InvalidValue);
	}
	HSV c = hsv();
	c.value = min<double>(c.value/factor, 1);
	return Color(c, _alpha).converTo(_model);
}

std::string Color::toHTMLString() const {
	if (_model == Rgb) {
		return (boost::format("#%1%%2%%3%")
			% toHtmlRGBComponentString(_rgb.red)
			% toHtmlRGBComponentString(_rgb.green)
			% toHtmlRGBComponentString(_rgb.blue)).str();
	}
	return converTo(Rgb).toHTMLString();
}

std::string Color::toString() const {
	boost::format fmt("%1%(%2%,%3%)");
	switch (_model) {
	case Cmyk:
		fmt = fmt % "cmyka" % (boost::format("%1%,%2%,%3%,%4%")
								% _cmyk.cyan
								% _cmyk.magenta
								% _cmyk.yellow
								% _cmyk.black).str();
		break;

	case Hsl:
		fmt = fmt % "hsla" % (boost::format("%1%,%2%,%3%")
								% _hsl.hue
								% _hsl.saturation
								% _hsl.lightness).str();
		break;

	case Hsv:
		fmt = fmt % "hsva" % (boost::format("%1%,%2%,%3%")
								% _hsv.hue
								% _hsv.saturation
								% _hsv.value).str();
		break;

	case Rgb:
		fmt = fmt % "rgba" % (boost::format("%1%,%2%,%3%")
								% _rgb.red
								% _rgb.green
								% _rgb.blue).str();
		break;

	default:
		return "invalid color";
	}

	return (fmt % _alpha).str();
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
