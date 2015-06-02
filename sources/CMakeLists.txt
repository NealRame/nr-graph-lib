cmake_minimum_required(VERSION 3.0.0)

set(
  NR_GRAPH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/NRGraph
)

# Recuperation des fichiers sources et des headers du projet AMMusic
set(HEADERS 
  ${NR_GRAPH_INCLUDE_DIR}/Brush.h
  ${NR_GRAPH_INCLUDE_DIR}/Color.h
  ${NR_GRAPH_INCLUDE_DIR}/Error.h
  ${NR_GRAPH_INCLUDE_DIR}/Gradient.h
  ${NR_GRAPH_INCLUDE_DIR}/Line.h
  ${NR_GRAPH_INCLUDE_DIR}/LinearGradient.h
  ${NR_GRAPH_INCLUDE_DIR}/Painter.h
  ${NR_GRAPH_INCLUDE_DIR}/Pen.h
  ${NR_GRAPH_INCLUDE_DIR}/Point.h
  ${NR_GRAPH_INCLUDE_DIR}/RadialGradient.h
  ${NR_GRAPH_INCLUDE_DIR}/Rectangle.h
  ${NR_GRAPH_INCLUDE_DIR}/Size.h
  ${NR_GRAPH_INCLUDE_DIR}/Surface.h
)
set(SOURCES
  Brush.cpp
  Color.cpp
  Error.cpp
  Gradient.cpp
  Line.cpp
  LinearGradient.cpp
  Painter.cpp
  Pen.cpp
  RadialGradient.cpp
  Rectangle.cpp
  Size.cpp
  Surface.cpp
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
