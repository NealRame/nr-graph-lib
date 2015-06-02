/// Color.h
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
class Color {
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
    Color();
    Color(Name);
    Color(const CMYK &, double alpha=1);
    Color(const RGB  &, double alpha=1);
    Color(const HSL  &, double alpha=1);
    Color(const HSV  &, double alpha=1);
    virtual ~Color();

public:
    bool isValid() const;

public:
    bool operator==(const Color &other) const;

public:
    Color converTo(Model) const;

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
    Color lighter(double) const;
    Color darker(double) const;

public:
    std::string toHTMLString() const;
    std::string toString() const;

private:
    Color(const void *);
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
std::ostream & operator<<(std::ostream &, const Color &);
} // namespace graph
} // namespace nealrame
} // namespace com
