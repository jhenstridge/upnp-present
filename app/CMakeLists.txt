file(GLOB QML_JS_FILES *.qml *.js)

add_custom_target(upnp-present-qmlfiles ALL SOURCES ${QML_JS_FILES})
install(FILES ${QML_JS_FILES} DESTINATION ${UPNP_PRESENT_DIR})
