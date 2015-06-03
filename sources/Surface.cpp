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

namespace {
    struct surface_deleter {
        void operator()(void *surface_ptr) {
            cairo_surface_destroy(reinterpret_cast<cairo_surface_t *>(surface_ptr));
        }
    };

}

struct surface::impl {
    std::unique_ptr<cairo_surface_t, ::surface_deleter> cairo_surface;

    impl(class size size, Backend backend) {
        switch (backend) {
            case Backend::Image:
                cairo_surface.reset(
                    cairo_image_surface_create(
                        CAIRO_FORMAT_ARGB32, size.width(), size.height()
                    )
                );
                break;

            default:
                error::raise(error::NotImplemented);
        }

        cairo_status_t status;
        if ((status = cairo_surface_status(cairo_surface.get())) != CAIRO_STATUS_SUCCESS) {
            error::raise(error::InternalError, cairo_status_to_string(status));
        }
    }

    class size size() const {
        auto surface_ptr = cairo_surface.get();
        return (class size){
            static_cast<double>(cairo_image_surface_get_width(surface_ptr)),
            static_cast<double>(cairo_image_surface_get_height(surface_ptr))
        };
    }

    void exportToPNG(const std::string &path) const {
        auto status = cairo_surface_write_to_png(cairo_surface.get(), path.data());
        switch(status) {
            case CAIRO_STATUS_SUCCESS:
                break;

            case CAIRO_STATUS_WRITE_ERROR:
                error::raise(error::IOError);
                break;

            case CAIRO_STATUS_SURFACE_TYPE_MISMATCH:
                error::raise(error::SurfaceTypeMismatch);
                break;

            case CAIRO_STATUS_NO_MEMORY:
                error::raise(error::NoMemory);
                break;

            default:
                error::raise(error::InternalError, cairo_status_to_string(status));
                break;
        }
    }
};

surface::surface(class size size, Backend backend) 
    : d(new impl(size, backend)) {
}

surface::~surface() {
}

size surface::size() const {
    return d->size();

}

void surface::export_to_PNG(const std::string &path) const {
    return d->exportToPNG(path);
}

void * surface::priv_data_() {
    return d->cairo_surface.get();
}
