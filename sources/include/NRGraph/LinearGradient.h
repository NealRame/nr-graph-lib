/// linear_gradient.h
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
class brush;
class Painter;
class linear_gradient : public gradient {
    friend class brush;
    friend class Painter;

public:
    linear_gradient();
    linear_gradient(const Point &start, const Point &end);
    linear_gradient(const linear_gradient &other);
    virtual ~linear_gradient();

public:
    virtual Type type() const override
    { return Type::Linear; }

public:
    Point start_point() const
    { return _start_point; }
    
    void set_start_point(const Point &point)
    { _start_point = point; }
    
    Point end_point() const
    { return _end_point; }
    
    void set_end_point(const Point &point)
    { _end_point = point; }

public:
    linear_gradient & operator=(const gradient &);

public:
    virtual std::string type_to_string() const;

protected:
    virtual std::shared_ptr<void> pattern_() const override;

private:
    linear_gradient(const void *);

private:
    Point _start_point;
    Point _end_point;
};
} // namespace graph
} // namespace nealrame
} // namespace com
