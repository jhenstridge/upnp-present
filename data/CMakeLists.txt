# This command figures out the target architecture for use in the manifest file
execute_process(
  COMMAND dpkg-architecture -qDEB_HOST_ARCH
  OUTPUT_VARIABLE CLICK_ARCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(manifest.json.in ${CMAKE_CURRENT_BINARY_DIR}/manifest.json)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/manifest.json
  DESTINATION ${CMAKE_INSTALL_PREFIX})

install(FILES "upnp-present.apparmor" DESTINATION ${DATA_DIR})

# Substitute variables in the desktop file
configure_file(${DESKTOP_FILE_NAME}.in ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME}.in)
intltool_merge_translations(
  "${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME}.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME}"
  ALL
  UTF8
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${DESKTOP_FILE_NAME} DESTINATION ${DESKTOP_DIR})
