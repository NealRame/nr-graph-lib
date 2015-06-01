/// Error.h
///
/// - Created on: Jun 8, 2013
/// -     Author: jux
#pragma once

#include <string>

namespace com {
namespace nealrame {
namespace graph {
class Error {
public:
    enum Status {
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
    static std::string statusToString(Error::Status);

public:
    static void raise(Status, std::string message = "");

public:
    Error(Status, std::string msg = "");
    virtual ~Error();

public:
    Status status;
    std::string message;
};
} // namespace graph
} // namespace nealrame
} // namespace com
