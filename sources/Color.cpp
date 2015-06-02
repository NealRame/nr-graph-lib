/*
 * Color.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: jux
 */
#include <NRGraph/Color.h>
#include <NRGraph/Error.h>

extern "C" {
#   include <cairo.h>
}

#include <algorithm>
#include <cassert>
#include <cmath>
#include <sstream>

#include <boost/format.hpp>
#include <boost/regex.hpp>

using namespace com::nealrame::graph;

template<typename T>
T max(T a, T b, T c) {
    return std::max<T>(std::max<T>(a, b), c);
}

template<typename T>
T min(T a, T b, T c) {
    return std::min<T>(std::min<T>(a, b), c);
}

template<typename MODEL>
bool isValid(const MODEL &m);

template<>
bool isValid<Color::CMYK>(const Color::CMYK &cmyk) {
    return cmyk.cyan >= 0 && cmyk.cyan <= 1
        && cmyk.magenta >= 0 && cmyk.magenta <= 1
        && cmyk.yellow >= 0 && cmyk.yellow <= 1
        && cmyk.black >= 0 && cmyk.black <= 1;
}

template<>
bool isValid<Color::HSL>(const Color::HSL &hsl) {
    return hsl.hue >= 0 && hsl.hue < 360
        && hsl.saturation >= 0 && hsl.saturation <= 1
        && hsl.lightness >= 0 && hsl.lightness <= 1;
}

template<>
bool isValid<Color::HSV>(const Color::HSV &hsv) {
    return hsv.hue >= 0 && hsv.hue < 360
        && hsv.saturation >= 0 && hsv.saturation <= 1
        && hsv.value >= 0 && hsv.value <= 1;
}

template<>
bool isValid<Color::RGB>(const Color::RGB &rgb) {
    return rgb.red >= 0 && rgb.red <= 1
        && rgb.green >= 0 && rgb.green <= 1
        && rgb.blue >= 0 && rgb.blue <= 1;
}

bool operator==(const Color::CMYK &lhs, const Color::CMYK &rhs) {
    return lhs.cyan == rhs.cyan
    && lhs.magenta == rhs.magenta
    && lhs.yellow == rhs.magenta
    && lhs.black == rhs.black;
}

bool operator==(const Color::HSL &lhs, const Color::HSL &rhs) {
    return lhs.hue == rhs.hue
    && lhs.saturation == rhs.saturation
    && lhs.lightness == rhs.lightness;
}

bool operator==(const Color::HSV &lhs, const Color::HSV &rhs) {
    return lhs.hue == rhs.hue
    && lhs.saturation == rhs.saturation
    && lhs.value == rhs.value;
}

bool operator==(const Color::RGB &lhs, const Color::RGB &rhs) {
    return lhs.red == rhs.red
    && lhs.green == rhs.green
    && lhs.blue == rhs.blue;
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
    const auto k = 1 - max(source.red, source.green, source.blue);
    return Color::CMYK{
        (1 - source.red - k)/(1 - k),
        (1 - source.green - k)/(1 - k),
        (1 - source.blue - k)/(1 - k),
        k
    };
}

template<>
Color::RGB convert(const Color::CMYK &source) {
    if (! isValid(source)) {
        Error::raise(Error::InvalidValue);
    }
    const auto k = 1 - source.black;
    return Color::RGB{
        k*(1 - source.cyan),
        k*(1 - source.magenta),
        k*(1 - source.yellow)
    };
}

template<>
Color::HSL convert(const Color::RGB &source) {
    if (! isValid(source)) {
        Error::raise(Error::InvalidValue);
    }

    Color::HSL hsl;
    const auto cmax = max(source.red, source.green, source.blue);
    const auto cmin = min(source.red, source.green, source.blue);
    const auto delta = cmax - cmin;

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
        hsl.hue = hsl.hue*60;
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

    const auto c = source.saturation*(1 - std::fabs(2*source.lightness - 1));
    const auto m = source.lightness - c/2;
    const auto x = c*(1 - std::fabs(std::fmod(source.hue/60, 2) - 1));
    const auto h = source.hue;

    if (h < 60)  return Color::RGB{c + m, x + m, m};
    if (h < 120) return Color::RGB{x + m, c + m, m};
    if (h < 180) return Color::RGB{m, c + m, x + m};
    if (h < 240) return Color::RGB{m, x + m, c + m};
    if (h < 300) return Color::RGB{x + m, m, c + m};

    return Color::RGB{c + m, m, x + m};
}

template<>
Color::HSV convert(const Color::RGB &source) {
    if (! isValid(source)) {
        Error::raise(Error::InvalidValue);
    }

    const auto cmax  = max<double>(source.red, source.green, source.blue);
    const auto cmin  = min<double>(source.red, source.green, source.blue);
    const auto delta = cmax - cmin;

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

    const auto c = source.value*source.saturation;
    const auto x = c*(1 - std::fabs(std::fmod(source.hue/60, 2) - 1));
    const auto m = source.value - c;
    const auto h = source.hue;

    if (h < 60)  return Color::RGB{c + m, x + m, m };
    if (h < 120) return Color::RGB{x + m, c + m, m };
    if (h < 180) return Color::RGB{m, c + m, x + m };
    if (h < 240) return Color::RGB{m, x + m, c + m };
    if (h < 300) return Color::RGB{x + m, m, c + m };

    return {c + m, m, x + m};
}

static std::string toHtmlRGBComponentString(double c) {
    const auto v = c*255;

    if (v > 255) {
        Error::raise(Error::InvalidValue);
    }

    std::stringstream ss;

    ss.width(2);
    ss.fill('0');
    ss << std::hex << std::noshowbase << v;

    return ss.str();
}

Color::Color()
    : _model(Rgb)
    , _rgb{0, 0, 0}
    , _alpha(1) {
}

Color::Color(Name name) :
    _model(Rgb),
    _alpha(1) {
    switch(name) {
    case Black:
        _rgb = RGB{0, 0, 0};
        break;

    case White:
        _rgb = RGB{1, 1, 1};
        break;

    case Red:
        _rgb = RGB{1, 0, 0};
        break;

    case Lime:
        _rgb = RGB{0, 1, 0};
        break;

    case Blue:
        _rgb = RGB{0, 0, 1};
        break;

    case Yellow:
        _rgb = RGB{1, 1, 0};
        break;

    case Cyan:
        _rgb = RGB{0, 1, 1};
        break;

    case Magenta:
        _rgb = RGB{1, 0, 1};
        break;

    case Silver:
        _rgb = RGB{0.75, 0.75, 0.75};
        break;

    case Gray:
        _rgb = RGB{0.5, 0.5, 0.5};
        break;

    case Maroon:
        _rgb = RGB{0.5, 0, 0};
        break;

    case Olive:
        _rgb = RGB{0.5, 0.5, 0};
        break;

    case Green:
        _rgb = RGB{0, 0.5, 0};
        break;

    case Purple:
        _rgb = RGB{0.5, 0, 0.5};
        break;

    case Teal:
        _rgb = RGB{0, 0.5, 0.5};
        break;

    case Navy:
        _rgb = RGB{0, 0, 0.5};
        break;
    }
}

Color::Color(const CMYK &cmyk, double alpha)
    : _model(Cmyk)
    , _cmyk{cmyk.cyan, cmyk.magenta, cmyk.yellow, cmyk.black}
    , _alpha(alpha) {
}

Color::Color(const struct RGB &rgb, double alpha)
    : _model(Rgb)
    , _rgb{rgb.red, rgb.green, rgb.blue}
    , _alpha(alpha) {
}

Color::Color(const struct HSL &hsl, double alpha)
    : _model(Hsl)
    , _hsl{hsl.hue, hsl.saturation, hsl.lightness}
    , _alpha(alpha) {
}

Color::Color(const struct HSV &hsv, double alpha)
    : _model(Hsv)
    , _hsv{hsv.hue, hsv.saturation, hsv.value}
    , _alpha(alpha) {
}

Color::Color(const void *ptr) {
    auto pattern = reinterpret_cast<cairo_pattern_t *>((void *)ptr);
    assert(::cairo_pattern_get_type(pattern) == CAIRO_PATTERN_TYPE_SOLID);
    _model = Rgb;
    ::cairo_pattern_get_rgba(pattern, &_rgb.red, &_rgb.green, &_rgb.blue, &_alpha);
}

Color::~Color() {
}

std::shared_ptr<void> Color::pattern_() const {
    const auto rgb = this->rgb();
    auto pattern = ::cairo_pattern_create_rgba(rgb.red, rgb.green, rgb.blue, _alpha);
    return std::shared_ptr<void>(pattern, ::cairo_pattern_destroy);
}

bool Color::isValid() const {
    switch (_model) {
        case Cmyk:
            return ::isValid(_cmyk);
        case Hsl:
            return ::isValid(_hsl);
        case Hsv:
            return ::isValid(_hsv);
        case Rgb:
            return ::isValid(_rgb);
        default:
            return false;
    }
}

bool Color::operator==(const Color &other) const {
    if (_model == other._model) {
        if (_alpha == other._alpha) {
            switch (_model) {
            case Cmyk: return _cmyk == other._cmyk;
            case Hsl:  return _hsl == other._hsl;
            case Hsv:  return _hsv == other._hsv;
            case Rgb:  return _rgb == other._rgb;
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
        case Hsl:  return Color(convert<Color::HSL, Color::CMYK>(this->_hsl),  _alpha);
        case Hsv:  return Color(convert<Color::HSV, Color::CMYK>(this->_hsv),  _alpha);
        case Rgb:  return Color(convert<Color::RGB, Color::CMYK>(this->_rgb),  _alpha);
        default:
            break;
        }
        break;
    case Hsl:
        switch (_model) {
        case Cmyk: return Color(convert<Color::CMYK, Color::HSL>(this->_cmyk), _alpha);
        case Hsv:  return Color(convert<Color::HSV, Color::HSL> (this->_hsv),  _alpha);
        case Rgb:  return Color(convert<Color::RGB, Color::HSL> (this->_rgb),  _alpha);
        default:
            break;
        }
        break;
    case Hsv:
        switch (_model) {
        case Cmyk: return Color(convert<Color::CMYK, Color::HSV>(this->_cmyk), _alpha);
        case Hsl:  return Color(convert<Color::HSL, Color::HSV> (this->_hsl),  _alpha);
        case Rgb:  return Color(convert<Color::RGB, Color::HSV> (this->_rgb),  _alpha);
        default:
            break;
        }
        break;
    case Rgb:
        switch (_model) {
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
    return *this;
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
    auto c = hsv();
    c.value = std::min<double>(c.value*factor, 1);
    return Color(c, _alpha).converTo(_model);
}

Color Color::darker(double factor) const {
    if (factor < 0) {
        Error::raise(Error::InvalidValue);
    }
    auto c = hsv();
    c.value = std::min<double>(c.value/factor, 1);
    return Color(c, _alpha).converTo(_model);
}

std::string Color::toHTMLString() const {
    if (_model == Rgb) {
        std::string res = 
            (boost::format("#%1%%2%%3%")
                % toHtmlRGBComponentString(_rgb.red)
                % toHtmlRGBComponentString(_rgb.green)
                % toHtmlRGBComponentString(_rgb.blue)).str();

        // If res match the given regex (i.e. #XXYYZZ) then we can 
        // simplify it (i.e. #XYZ) 
        boost::regex re("^.(.)\\1(.)\\2(.)\\3$");
        boost::smatch m;

        return boost::regex_match(res, m, re) ?
            (boost::format("#%1%%2%%3%") % m[1].str() % m[2].str() % m[3].str()).str() : res;
    }
    return converTo(Rgb).toHTMLString();
}

std::string Color::toString() const {
    if (_alpha == 1) {
        switch (_model) {
        case Cmyk:
            return (boost::format("cmyk(%1%,%2%,%3%,%4%)")
                    % _cmyk.cyan
                    % _cmyk.magenta
                    % _cmyk.yellow
                    % _cmyk.black).str();
        case Hsl:
            return (boost::format("hsl(%1%,%2%,%3%)")
                    % _hsl.hue
                    % _hsl.saturation
                    % _hsl.lightness).str();

        case Hsv:
            return (boost::format("hsv(%1%,%2%,%3%)")
                    % _hsv.hue
                    % _hsv.saturation
                    % _hsv.value).str();

        case Rgb:
            return toHTMLString();

        default:
            return "invalid color";
        }   
    } else {
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
}

namespace com {
namespace nealrame {
namespace graph {

std::ostream & operator<<(std::ostream &out, const Color &color)  {
    return out << color.toString();
}

} /* namespace graph */
} /* namespace nealrame */
} /* namespace com */
