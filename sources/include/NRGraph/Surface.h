/// GraphSurface.h
///
/// - Created on: 11 juin 2013
/// -     Author: jux
#pragma once

#include <NRGraph/size.h>

#include <memory>
#include <string>

namespace com {
namespace nealrame {
namespace graph {
class painter;
class surface {
    friend class painter;
    
public:
    enum class backend {
        Image,
        Pdf,
        Svg,
        Window
    };

public:
    surface(size, enum backend backend);
    virtual ~surface();

public:
    size size() const;

public:
    void export_to_PNG(const std::string &) const;

private:
    void * priv_data_();

private:
    struct impl;
    std::unique_ptr<impl> d;
};

} // namespace graph
} // namespace nealrame
} // namespace com
