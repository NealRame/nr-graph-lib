/// brush.h
///
/// -  Created on: 12 juin 2013
/// -     Author: jux
#pragma once

#include <NRGraph/Color.h>
#include <NRGraph/Gradient.h>

#include <iostream>
#include <memory>
#include <string>

namespace com {
namespace nealrame {
namespace graph {
class brush {
    friend class Painter;

public:
    enum class type {
        Gradient,
        Solid,
        Surface,
        Null,
    };

public:
    brush();
    brush(const color &);
    brush(const color::Name);
    brush(const Gradient &);
    brush(const brush &);
    brush(brush &&);
    virtual ~brush();

public:
    brush & operator=(const color &);
    brush & operator=(const color::Name &);
    brush & operator=(const Gradient &);
    brush & operator=(const brush &);
    brush & operator=(brush &&);

public:
    bool is_null() const {
        return type() == type::Null;
    }

public:
    enum type type() const {
        return type_;
    }

public:
    color & color();
    const class color & color() const;
    void set_color(const class color &);

    Gradient & gradient();
    const Gradient & gradient() const;
    void set_gradient(const Gradient &);

public:
    std::string to_string() const;

private:
    brush(const void *);
    std::shared_ptr<void> pattern_() const;

private:
    enum type type_;
    struct impl;
    std::unique_ptr<impl> d;
};

std::ostream & operator<<(std::ostream &, const brush &);

} // namespace graph
} // namespace nealrame
} // namespace com
