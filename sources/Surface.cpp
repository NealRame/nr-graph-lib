/*
 * GraphSurface.cpp
 *
 *  Created on: 11 juin 2013
 *      Author: jux
 */

#include <NRGraph/Error.h>
#include <NRGraph/Surface.h>

 extern "C" {
#   include <cairo.h>
}

#include <string>

#define CAIRO_SURFACE(PTR) reinterpret_cast<cairo_surface_t *>(PTR)

using namespace com::nealrame::graph;

Surface::Surface(Size size, Backend backend) {
    int w = static_cast<int>(size.width()),
        h = static_cast<int>(size.height());

    cairo_surface_t *surface = nullptr;

    switch (backend) {
    case Backend::Image:
        surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
        break;
    default:
        Error::raise(Error::NotImplemented);
    }

    cairo_status_t status;

    if ((status = cairo_surface_status(surface)) != CAIRO_STATUS_SUCCESS) {
        Error::raise(Error::InternalError, cairo_status_to_string(status));
    }

    _priv_data = surface;
}

Surface::~Surface() {
    auto surface = CAIRO_SURFACE(_priv_data);
    cairo_surface_destroy(surface);
}

Size Surface::size() const {
    auto surface = CAIRO_SURFACE(_priv_data);
    return Size(static_cast<double>(cairo_image_surface_get_width(surface)),
                static_cast<double>(cairo_image_surface_get_height(surface)));
}

void Surface::exportToPNG(const std::string &path) const {
    cairo_status_t status;

    if ((status = cairo_surface_write_to_png(CAIRO_SURFACE(_priv_data), path.data())) != CAIRO_STATUS_SUCCESS) {
        switch(status) {
        case CAIRO_STATUS_WRITE_ERROR:
            Error::raise(Error::IOError);
            break;

        case CAIRO_STATUS_SURFACE_TYPE_MISMATCH:
            Error::raise(Error::SurfaceTypeMismatch);
            break;

        case CAIRO_STATUS_NO_MEMORY:
            Error::raise(Error::NoMemory);
            break;

        default:
            Error::raise(Error::InternalError, cairo_status_to_string(status));
            break;
        }
    }
}
