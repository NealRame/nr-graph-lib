/// Error.h
///
/// - Created on: Jun 8, 2013
/// -     Author: jux
#pragma once

#include <string>

namespace com {
namespace nealrame {
namespace graph {
class error {
public:
    enum status {
        BrushTypeMismatch,
        InternalError,
        InvalidColorStopIndex,
        InvalidValue,
        IOError,
        NoMemory,
        NotImplemented,
        SurfaceTypeMismatch,
    };

public:
    static std::string status_to_string(error::status);

public:
    static void raise(status, std::string message = "");

public:
    error(status, std::string msg = "");
    virtual ~error();

public:
    enum status status;
    std::string message;
};
} // namespace graph
} // namespace nealrame
} // namespace com
