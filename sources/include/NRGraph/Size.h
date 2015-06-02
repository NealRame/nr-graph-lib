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
class Size {
public:
    Size()
        : _width(0)
        , _height(0) {
    }
    Size(double width, double height)
        : _width(width)
        , _height(height) {
    }
    virtual ~Size() { }

    /// Returns a size holding the minimum width and height of this size and
    /// the given otherSize.
    Size boundedTo(const Size & otherSize) const;

    /// Returns a size holding the maximum width and height of this size and
    /// the given otherSize.
    Size expandedTo(const Size & otherSize) const;

    double & height()
    { return _height; }
    
    const double & height() const
    { return _height; }
    
    double & width()
    { return _width; }
    
    const double & width() const
    { return _width; }

    bool isValid() const;

public:
    virtual std::string toString() const;

private:
    double _width;
    double _height;
};

std::ostream & operator<<(std::ostream &, const Size &);

} // namespace graph
} // namespace nealrame
} // namespace com
