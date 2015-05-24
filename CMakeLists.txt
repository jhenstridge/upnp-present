project(upnp-present C CXX)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include(FindPkgConfig)
find_package(Qt5Core REQUIRED)
add_definitions(-DQT_NO_KEYWORDS)
pkg_check_modules(GUPNP REQUIRED gupnp-1.0 gupnp-av-1.0)

add_subdirectory(src)
