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
add_custom_target(upnp-qmldir ALL
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_CURRENT_BINARY_DIR}/qmldir
  DEPENDS qmldir upnp-model
)

add_custom_command(
  TARGET upnp-qmldir
  COMMAND QML2_IMPORT_PATH=${CMAKE_BINARY_DIR}/src qmlplugindump upnp 0.1 > ${CMAKE_CURRENT_BINARY_DIR}/plugin.qmltypes
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src
)

install(TARGETS upnp-model DESTINATION ${QT_IMPORTS_DIR}/upnp)
install(FILES qmldir plugin.qmltypes DESTINATION ${QT_IMPORTS_DIR}/upnp)
