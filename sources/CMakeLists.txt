cmake_minimum_required(VERSION 3.0.0)

set(
  NR_GRAPH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/NRGraph
)

# Recuperation des fichiers sources et des headers du projet AMMusic
set(HEADERS 
  ${NR_GRAPH_INCLUDE_DIR}/brush.h
  ${NR_GRAPH_INCLUDE_DIR}/color.h
  ${NR_GRAPH_INCLUDE_DIR}/error.h
  ${NR_GRAPH_INCLUDE_DIR}/gradient.h
  ${NR_GRAPH_INCLUDE_DIR}/line.h
  ${NR_GRAPH_INCLUDE_DIR}/linear_gradient.h
  ${NR_GRAPH_INCLUDE_DIR}/painter.h
  ${NR_GRAPH_INCLUDE_DIR}/pen.h
  ${NR_GRAPH_INCLUDE_DIR}/point.h
  ${NR_GRAPH_INCLUDE_DIR}/radial_gradient.h
  ${NR_GRAPH_INCLUDE_DIR}/rectangle.h
  ${NR_GRAPH_INCLUDE_DIR}/size.h
  ${NR_GRAPH_INCLUDE_DIR}/surface.h
)
set(SOURCES
  brush.cpp
  color.cpp
  error.cpp
  gradient.cpp
  line.cpp
  linear_gradient.cpp
  painter.cpp
  pen.cpp
  radial_gradient.cpp
  rectangle.cpp
  size.cpp
  surface.cpp
)

# Definition des fichiers sources src/*.h|cpp include/core/*.h
set(ALL_SRCS ${SOURCES} ${HEADERS})

add_library(${PROJECT_NAME} STATIC ${ALL_SRCS})
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
    ${Boost_INCLUDE_DIRS}
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
    ${Boost_LIBRARIES}
    Cairo::Cairo
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

# Install libraries
install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION "${NR_GRAPH_LIB_INSTALL_DIR}/lib"
  LIBRARY DESTINATION "${NR_GRAPH_LIB_INSTALL_DIR}/lib"
)

# Install includes
install(
  DIRECTORY ${NR_GRAPH_INCLUDE_DIR}
  DESTINATION ${NR_GRAPH_LIB_INSTALL_DIR}/include
)
