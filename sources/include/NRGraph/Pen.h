/// GraphPen.h
///
/// - Created on: Jun 12, 2013
/// -     Author: jux
#pragma once

namespace com {
namespace nealrame {
namespace graph {
class Pen {
public:
    Pen();
    virtual ~Pen();

public:
    double width() const
    { return _width;  }
    
    void setWidth(double width)
    { _width = width; }

private:
    double _width;
};

} // namespace graph
} // namespace nealrame
} // namespace com
