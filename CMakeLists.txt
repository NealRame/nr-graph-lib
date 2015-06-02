cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(NRGraph)

###
### Project global variables
set(
  NR_GRAPH_LIB_INSTALL_DIR
  "${CMAKE_INSTALL_PREFIX}/NRGraph"
  CACHE PATH "Installation directory for libraries"
)

if(APPLE)
  set(COMMON_CXX_FLAGS "-std=c++11 -stdlib=libc++ -ferror-limit=0 -Wall -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_CXX_FLAGS} -g -O0")
  set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O3")
else()
  # TODO add compile flags for Visual Studio 
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

###
### Project dependency
find_package(Boost 1.58.0 REQUIRED)
find_package(Cairo REQUIRED)

###
### Project
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/sources")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")

###
### Generate Sublime Text project file
if(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
  include(SublimeTextProject)

  # get includes directories list
  get_property(DIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
  get_directory_property(DEFS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMPILE_DEFINITIONS)

  list(
    APPEND DIRS
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/include
    ${LIBCAIRO_INCLUDE_DIRS}
  )

  # only transform COMMON_CXX_FLAGS into a list
  string(REGEX MATCHALL "[^ ]+" FLAGS ${COMMON_CXX_FLAGS})

  generate_sublime_text_project("${FLAGS}" "${DEFS}" "${DIRS}")
endif()
