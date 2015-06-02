/*
 * error.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: jux
 */

#include <NRGraph/error.h>

using namespace com::nealrame::graph;

std::string error::statusToString(enum error::status status) {
    switch (status) {
    case error::status::BrushTypeMismatch:
        return "graph::BrushTypeMismatch";

    case error::status::InternalError:
        return "graph::InternalError";

    case error::status::InvalidColorStopIndex:
        return "graph::InvalidColorStopIndex";

    case error::status::InvalidValue:
        return "graph::InvalidValue";

    case error::status::IOError:
        return "graph::IOError";

    case error::status::NoMemory:
        return "graph::NoMemory";

    case error::status::NotImplemented:
        return "graph::NotImplemented";

    case error::status::SurfaceTypeMismatch:
        return "graph::SurfaceTypeMismatch";

    default:
        return "";
    }
}

error::error(enum status s, std::string msg)
    : status(s)
    , message(msg) {
}

error::~error() {
}

void error::raise(enum status status, std::string msg) {
    throw error(status, msg);
}
