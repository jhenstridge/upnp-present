
add_definitions(${GUPNP_CFLAGS})

add_library(upnp-model STATIC
  control-point-model.cpp
  )
set_target_properties(upnp-model PROPERTIES AUTOMOC TRUE)
qt5_use_modules(upnp-model Core)
target_link_libraries(upnp-model ${GUPNP_LDFLAGS})
