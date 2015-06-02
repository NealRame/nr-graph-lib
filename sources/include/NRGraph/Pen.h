/// GraphPen.h
///
/// - Created on: Jun 12, 2013
/// -     Author: jux
#pragma once

namespace com {
namespace nealrame {
namespace graph {
class pen {
public:
    pen();
    virtual ~pen();

public:
    double width() const
    { return _width;  }
    
    void set_width(double width)
    { _width = width; }

private:
    double _width;
};

} // namespace graph
} // namespace nealrame
} // namespace com
