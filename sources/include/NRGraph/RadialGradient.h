/// radial_gradient.h
///
/// - Created on: 13 juin 2013
/// -     Author: jux
#pragma once

#include <NRGraph/Gradient.h>
#include <NRGraph/Point.h>

#include <memory>

namespace com {
namespace nealrame {
namespace graph {
class brush;
class Painter;
class point;
class radial_gradient : public gradient {
    friend class brush;
    friend class Painter;

public:
    radial_gradient(const point &center, double radius);
    radial_gradient(const point &starCenter, double startRadius, const point &endCenter, double endRadius);
    radial_gradient(const radial_gradient &);
    virtual ~radial_gradient();

public:
    virtual enum type type() const override
    { return gradient::type::Radial; }

public:
    point start_circle_center_point() const
    { return _start_circle_center; }
    void set_start_circle_center_point(const point &p)
    { _start_circle_center = p; }

    double start_circle_radius() const
    { return _start_circle_radius; }
    void set_start_circle_radius(double radius)
    { _start_circle_radius = radius; }

    point end_circle_center_point() const
    { return _end_circle_center; }
    void set_end_circle_center_point(const point &p)
    { _end_circle_center = p; }

    double end_circle_radius() const
    { return _end_circle_radius; }
    void set_end_circle_radius(double radius)
    { _end_circle_radius = radius; }

public:
    radial_gradient & operator=(const gradient &);

public:
    virtual std::string type_to_string() const;

protected:
    virtual std::shared_ptr<void> pattern_() const override;

private:
    radial_gradient(const void *);

private:
    point _start_circle_center;
    double _start_circle_radius;
    point _end_circle_center;
    double _end_circle_radius;
};

} // namespace graph
} // namespace nealrame
} // namespace com
