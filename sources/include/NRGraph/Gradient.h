/// Graphgradient.h
///
/// - Created on: Jun 12, 2013
/// -     Author: jux
#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <NRGraph/color.h>

namespace com {
namespace nealrame {
namespace graph {
class brush;
class gradient {
    friend class brush;
public:
    enum class type {
        Linear,
        Radial
    };

    enum class extend {
        None,
        Pad,
        Repeat,
        Reflect
    };

    struct stop {
        double offset;
        class color color;
    };

public:
    gradient();
    gradient(const gradient &);
    virtual ~gradient();

public:
    virtual enum type type() const = 0;
    
    virtual enum extend extend() const
    { return _extend; }
    
    virtual void set_extend(enum extend extend)
    { _extend = extend; }

public:
    virtual void add_color_stop(double, const color &);
    virtual void add_color_stop(const stop &);
    virtual void add_color_stops(const std::vector<stop> &);
    virtual void set_color_stops(const std::vector<stop> &);

    virtual stop color_stop(unsigned int index) const;
    virtual unsigned int color_stop_count() const  { return _stops.size(); }
    virtual std::vector<stop> color_stops() const { return _stops; }

public:
    virtual gradient & operator=(const gradient &);
    virtual bool operator==(const gradient &);

public:
    virtual std::string type_to_string() const;
    virtual std::string to_string() const;

protected:
    virtual std::shared_ptr<void> pattern_() const = 0;

protected:
    enum extend _extend;
    std::vector<stop> _stops;
};

std::ostream & operator<<(std::ostream &, const gradient &);
} // namespace graph
} // namespace nealrame
} // namespace com
