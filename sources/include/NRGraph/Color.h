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
    enum model {
        Cmyk, Hsl, Hsv, Rgb, Invalid
    };

    enum name {
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
    color(name);
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
    color conver_to(model) const;

public:
    double alpha() const;
    void set_alpha(double);

    enum model model() const;
    void set_model(enum model);

    CMYK cmyk() const;
    void set_cmyk(const CMYK &);

    RGB rgb() const;
    void set_rgb(const RGB &);

    HSL hsl() const;
    void set_hsl(const HSL &);

    HSV hsv() const;
    void set_hsv(const HSL &);

public:
    color lighter(double) const;
    color darker(double) const;

public:
    std::string to_HTML_string() const;
    std::string to_string() const;

private:
    color(const void *);
    std::shared_ptr<void> pattern_() const;

private:
    enum model _model;
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
