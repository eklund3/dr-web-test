set(APP_TARGET "${PROJECT_NAME}-app")
set(APP_DATA_DIR "${PROJECT_SOURCE_DIR}/data")
set(APP_ICON "${APP_DATA_DIR}/icons/icons8_change_theme.ico")
set(APP_COMPANY_NAME "DRWEB")

add_executable(${APP_TARGET} WIN32
    "main.cpp"
    ${VERSION_FILES}
)

set_target_properties(${APP_TARGET} PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
)

target_link_libraries(${APP_TARGET} PRIVATE drweb::${PROJECT_NAME})
