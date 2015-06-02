/// GraphLinearGradient.h
///
/// - Created on: 13 juin 2013
/// -     Author: jux

#pragma once

#include <NRGraph/Point.h>
#include <NRGraph/Gradient.h>

#include <memory>

namespace com {
namespace nealrame {
namespace graph {
class Painter;
class LinearGradient : public Gradient {
    friend class Brush;
    friend class Painter;

public:
    LinearGradient();
    LinearGradient(const Point &start, const Point &end);
    LinearGradient(const LinearGradient &other);
    virtual ~LinearGradient();

public:
    virtual Type type() const override
    { return Gradient::Type::Linear; }

public:
    Point startPoint() const
    { return _start_point; }
    
    void setStartPoint(const Point &point)
    { _start_point = point; }
    
    Point endPoint() const
    { return _end_point; }
    
    void setEndPoint(const Point &point)
    { _end_point = point; }

public:
    LinearGradient & operator=(const Gradient &);

public:
    virtual std::string typeToString() const;

protected:
    virtual std::shared_ptr<void> pattern_() const override;

private:
    LinearGradient(const void *);

private:
    Point _start_point;
    Point _end_point;
};
} // namespace graph
} // namespace nealrame
} // namespace com
