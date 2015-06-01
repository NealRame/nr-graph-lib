## FindCAIRO
## -----------
##
## Try to find the lib Cairo.
##
## Once done this will define:
## - Variables:
##   * `LIBCAIRO_FOUND` - System has CAIRO.
##   * `LIBCAIRO_INCLUDE_DIRS` - The CAIRO include directory.
##   * `LIBCAIRO_LIBRARY` - The libraries needed to use CAIRO.
##   * `LIBCAIRO_VERSION_STRING` - the version of CAIRO found (since CMake 2.8.8).
## - Targets:
##   * `Cairo::Cairo` - Target used to link against de library.

if(UNIX)
  if(APPLE)
    set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/lib/pkgconfig:/opt/X11/lib/pkgconfig")
  endif()
  find_package(PkgConfig QUIET)
  if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_CAIRO QUIET cairo)
    find_library(
      LIBCAIRO_LIBRARY
      NAMES
        cairo
      HINTS
        ${PC_CAIRO_LIBRARY_DIRS}
      PATH_SUFFIXES
        lib
    )
    set(LIBCAIRO_INCLUDE_DIRS "${PC_CAIRO_INCLUDE_DIRS}")
    set(LIBCAIRO_VERSION_STRING "${PC_CAIRO_VERSION}")
  endif()
endif()

# export a target
if(NOT TARGET Cairo::Cairo)
  add_library(Cairo::Cairo UNKNOWN IMPORTED)
  set_target_properties(
    Cairo::Cairo
    PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${LIBCAIRO_INCLUDE_DIRS}"
      IMPORTED_LOCATION "${LIBCAIRO_LIBRARY}"
  )
endif()

# handle the QUIETLY and REQUIRED arguments and set LIBCAIRO_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  LIBCAIRO
  REQUIRED_VARS LIBCAIRO_LIBRARY LIBCAIRO_INCLUDE_DIRS
  VERSION_VAR LIBCAIRO_VERSION_STRING
)

mark_as_advanced(LIBCAIRO_INCLUDE_DIRS LIBCAIRO_LIBRARY LIBCAIRO_VERSION_STRING)
