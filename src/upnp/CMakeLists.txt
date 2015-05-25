add_definitions(${GUPNP_CFLAGS})

add_library(upnp-model MODULE
  plugin.cpp
  control-point-model.cpp
  media-renderer.cpp
  )
set_target_properties(upnp-model PROPERTIES AUTOMOC TRUE)
qt5_use_modules(upnp-model Core Qml)
target_link_libraries(upnp-model ${GUPNP_LDFLAGS})

configure_file(
  qmldir
  qmldir)
