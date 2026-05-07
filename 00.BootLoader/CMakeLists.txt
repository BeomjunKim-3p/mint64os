add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/BootLoader.bin
    COMMAND nasm -o ${CMAKE_CURRENT_BINARY_DIR}/BootLoader.bin ${CMAKE_CURRENT_SOURCE_DIR}/BootLoader.asm
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/BootLoader.asm
    COMMENT "Assembling BootLoader..."
)
add_custom_target(BootLoader DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/BootLoader.bin)