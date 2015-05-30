add_definitions(-DQT_NO_KEYWORDS ${GUPNP_CFLAGS})

add_library(upnp-model MODULE
  plugin.cpp
  control-point-model.cpp
  media-renderer.cpp
  )
set_target_properties(upnp-model PROPERTIES AUTOMOC TRUE)
qt5_use_modules(upnp-model Core Qml)
target_link_libraries(upnp-model ${GUPNP_LDFLAGS})

# Copy qmldir file to build dir for running in QtCreator
configure_file(
  qmldir
  qmldir)

install(TARGETS upnp-model DESTINATION ${QT_IMPORTS_DIR}/upnp)
install(FILES qmldir DESTINATION ${QT_IMPORTS_DIR}/upnp)
