/*
 * Error.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: jux
 */

#include <NRGraph/Error.h>

using namespace com::nealrame::graph;

std::string Error::statusToString(Error::Status status) {
    switch (status) {
    case Error::Status::BrushTypeMismatch:
        return "graph::BrushTypeMismatch";

    case Error::Status::InternalError:
        return "graph::InternalError";

    case Error::Status::InvalidColorStopIndex:
        return "graph::InvalidColorStopIndex";

    case Error::Status::InvalidValue:
        return "graph::InvalidValue";

    case Error::Status::IOError:
        return "graph::IOError";

    case Error::Status::NoMemory:
        return "graph::NoMemory";

    case Error::Status::NotImplemented:
        return "graph::NotImplemented";

    case Error::Status::SurfaceTypeMismatch:
        return "graph::SurfaceTypeMismatch";

    default:
        return "";
    }
}

Error::Error(Status s, std::string msg)
    : status(s)
    , message(msg)
{ }

Error::~Error()
{ }

void Error::raise(Status status, std::string msg) {
    throw Error(status, msg);
}
