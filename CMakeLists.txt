#-------------------
# Main elf
#-------------------
file(GLOB MAIN_SRC
    "${PROJECT_SOURCE_DIR}/app/edge_impulse_inference/*.c"
    "${PROJECT_SOURCE_DIR}/target/*.c"
    "${PROJECT_SOURCE_DIR}/lib/GNSE_BSP/*.c"
    )
set(SOURCES
    ${MAIN_SRC}
    ${PROJECT_SOURCE_DIR}/${STARTUP_FILE}
    ${PROJECT_SOURCE_DIR}/${SYSTEM_FILE}
    )
add_executable(${PROJECT_NAME}.elf
    ${SOURCES}
    )
target_include_directories(${PROJECT_NAME}.elf
    PUBLIC
    ${PROJECT_SOURCE_DIR}/target
    ${PROJECT_SOURCE_DIR}/app/edge_impulse_inference
    ${PROJECT_SOURCE_DIR}/app/edge_impulse_inference/conf
    ${PROJECT_SOURCE_DIR}/lib/GNSE_BSP
    )
target_link_libraries(${PROJECT_NAME}.elf
    PUBLIC
    hal
    )
