/// color.h
///
///  Created on: Jun 8, 2013
///      Author: jux
#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace com {
namespace nealrame {
namespace graph {
class color {
    friend class brush;
    friend class Painter;

public:
    enum Model {
        Cmyk, Hsl, Hsv, Rgb, Invalid
    };

    enum Name {
        Black,
        White,
        Red,
        Lime,
        Blue,
        Yellow,
        Cyan,
        Magenta,
        Silver,
        Gray,
        Maroon,
        Olive,
        Green,
        Purple,
        Teal,
        Navy
    };

    struct CMYK {
        double cyan;
        double magenta;
        double yellow;
        double black;
    };

    struct RGB {
        double red;
        double green;
        double blue;
    };

    struct HSL {
        double hue;
        double saturation;
        double lightness;
    };

    struct HSV {
        double hue;
        double saturation;
        double value;
    };

public:
    color();
    color(Name);
    color(const CMYK &, double alpha=1);
    color(const RGB  &, double alpha=1);
    color(const HSL  &, double alpha=1);
    color(const HSV  &, double alpha=1);
    virtual ~color();

public:
    bool isValid() const;

public:
    bool operator==(const color &other) const;

public:
    color converTo(Model) const;

public:
    double alpha() const;
    void setAlpha(double);

    Model model() const;
    void setModel(Model);

    CMYK cmyk() const;
    void setCmyk(const CMYK &);

    RGB rgb() const;
    void setRgb(const RGB &);

    HSL hsl() const;
    void setHsl(const HSL &);

    HSV hsv() const;
    void setHsv(const HSL &);

public:
    color lighter(double) const;
    color darker(double) const;

public:
    std::string toHTMLString() const;
    std::string toString() const;

private:
    color(const void *);
    std::shared_ptr<void> pattern_() const;

private:
    Model _model;
    union {
        struct CMYK _cmyk;
        struct HSL  _hsl;
        struct HSV  _hsv;
        struct RGB  _rgb;
    };
    double _alpha;
};
std::ostream & operator<<(std::ostream &, const color &);
} // namespace graph
} // namespace nealrame
} // namespace com
