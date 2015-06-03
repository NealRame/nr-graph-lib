/// GraphSize.h
///
/// - Created on: Jun 9, 2013
/// -     Author: jux
#pragma once

#include <iostream>
#include <string>

namespace com {
namespace nealrame {
namespace graph {
class size {
public:
    size()
        : _width(0)
        , _height(0) {
    }
    size(double width, double height)
        : _width(width)
        , _height(height) {
    }
    virtual ~size() { }

    /// Returns a size holding the minimum width and height of this size and
    /// the given otherSize.
    size bounded_to(const size & otherSize) const;

    /// Returns a size holding the maximum width and height of this size and
    /// the given otherSize.
    size expanded_to(const size & otherSize) const;

    double & height()
    { return _height; }
    
    const double & height() const
    { return _height; }
    
    double & width()
    { return _width; }
    
    const double & width() const
    { return _width; }

    bool is_valid() const;

public:
    virtual std::string to_string() const;

private:
    double _width;
    double _height;
};

std::ostream & operator<<(std::ostream &, const size &);

} // namespace graph
} // namespace nealrame
} // namespace com
