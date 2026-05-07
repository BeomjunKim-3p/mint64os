set(K64_TEMP ${CMAKE_CURRENT_BINARY_DIR}/Temp)
file(MAKE_DIRECTORY ${K64_TEMP})

file(GLOB_RECURSE K64_CSOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.c")
file(GLOB_RECURSE K64_ASOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.asm")

# EntryPoint.s를 별도로 빌드 (NASM)
add_custom_command(
    OUTPUT ${K64_TEMP}/EntryPoint.o
    COMMAND nasm -f elf64 -o ${K64_TEMP}/EntryPoint.o ${CMAKE_CURRENT_SOURCE_DIR}/Source/EntryPoint.s
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Source/EntryPoint.s
    COMMENT "Building EntryPoint.o for Kernel64"
)

# 64비트 오브젝트 빌드 (EntryPoint.s 제외)
add_library(K64_Objs OBJECT ${K64_CSOURCES} ${K64_ASOURCES})
target_compile_options(K64_Objs PRIVATE 
    $<$<COMPILE_LANGUAGE:C>:-m64 -g -ffreestanding -nostdlib>
)
target_compile_options(K64_Objs PRIVATE 
    $<$<COMPILE_LANGUAGE:ASM_NASM>:-f elf64>
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Kernel64.bin
    COMMAND ${LD64} -melf_x86_64 -T ${CMAKE_CURRENT_SOURCE_DIR}/elf_x86_64.x -nostdlib -e Main -Ttext 0x200000 -o ${K64_TEMP}/Kernel64.elf ${K64_TEMP}/EntryPoint.o "$<TARGET_OBJECTS:K64_Objs>"
    COMMAND ${OC64} -j .text -j .data -j .rodata -j .bss -S -O binary ${K64_TEMP}/Kernel64.elf ${CMAKE_CURRENT_BINARY_DIR}/Kernel64.bin
    DEPENDS ${K64_TEMP}/EntryPoint.o K64_Objs
    COMMAND_EXPAND_LISTS
)

add_custom_target(Kernel64 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Kernel64.bin)