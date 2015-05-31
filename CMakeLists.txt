project(upnp-present C CXX)
cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Do not remove this line, its required for the correct functionality of the Ubuntu-SDK
set(UBUNTU_MANIFEST_PATH "data/manifest.json.in" CACHE INTERNAL "Tells QtCreator loca
tion and name of the manifest file")

set(APP_NAME          upnp-present)
set(APP_ID            "upnp-present.jamesh")
set(UPNP_PRESENT_DIR "share/qml/upnp-present")
set(MAIN_QML          "upnp-present.qml")
set(ICON              "upnp-present.png")

set(GETTEXT_PACKAGE   "upnp-present")

# Set install paths
set(CMAKE_INSTALL_PREFIX /)
set(DATA_DIR ${CMAKE_INSTALL_PREFIX})
set(DESKTOP_DIR ${DATA_DIR})
set(DESKTOP_FILE_NAME "upnp-present.desktop")

set(EXEC "qmlscene $@ ${UPNP_PRESENT_DIR}/${MAIN_QML}")

# Components PATH
execute_process(
    COMMAND dpkg-architecture -qDEB_HOST_MULTIARCH
    OUTPUT_VARIABLE ARCH_TRIPLET
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(LIB_DIR "lib/${ARCH_TRIPLET}")
set(QT_IMPORTS_DIR "${LIB_DIR}/qt5/qml")

include(FindPkgConfig)
find_package(Intltool REQUIRED)
find_package(Qt5Core REQUIRED)
pkg_check_modules(GUPNP REQUIRED gupnp-1.0 gupnp-av-1.0)

add_subdirectory(po)
add_subdirectory(data)
add_subdirectory(src/upnp)
add_subdirectory(app)

