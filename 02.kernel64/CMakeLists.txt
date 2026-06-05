set(CC64 "${CC64};-c;-m64;-ffreestanding")
set(LD64 "${LD64};-melf_x86_64;-T;${CMAKE_CURRENT_SOURCE_DIR}/elf_x86_64.x;-nostdlib;-e;Main;-0x200000")
set(OC64 "${OC64};-j;.text;-j;.data;-j;.rodata;-j;.bss;-S;-O;binary")

#set(K64_TEMP ${CMAKE_CURRENT_BINARY_DIR}/Temp)
#file(MAKE_DIRECTORY ${K64_TEMP})

#file(GLOB_RECURSE K64_CSOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.c")
#file(GLOB_RECURSE K64_ASOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.asm")

set(K64_SOURCES	
	Private/Main.c
	Private/Descriptor.c
	Private/Utility.c
	Private/AssemblyUtility.asm
)
set(K64_HEADERS
	Public/AssemblyUtility.h
	Public/Types.h 
	Public/Utility.h 
	Private/Descriptor.h 
)


add_library(K64_Core STATIC
	${K64_SOURCES}
	${K64_HEADERS}
)

target_compile_options(K64_Core PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-m64 -g -ffreestanding -nostdlib>
    $<$<COMPILE_LANGUAGE:ASM_NASM>:-f elf64>
)

target_include_directories(K64_Core
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/Public
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/Private
)

add_subdirectory(Keyboard)
add_subdirectory(Console)

set(K64_MODULES
	Console
	Keyboard
)

target_link_libraries(K64_Core PRIVATE ${K64_MODULES})

# EntryPoint.s를 별도로 빌드 (NASM)
add_custom_command(
    OUTPUT EntryPoint.o
    COMMAND nasm -f elf64 -o EntryPoint.o ${CMAKE_CURRENT_SOURCE_DIR}/Private/EntryPoint.s
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Private/EntryPoint.s
    COMMENT "Building EntryPoint.o for Kernel64"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Kernel64.Elf
    COMMAND gcc -m64 -nostdlib
    	    -Wl,-melf_x86_64 
            -T ${CMAKE_CURRENT_SOURCE_DIR}/elf_x86_64.x 
            -e Main -Ttext 0x200000 
            -o ${CMAKE_CURRENT_BINARY_DIR}/Kernel64.Elf
            ${CMAKE_CURRENT_BINARY_DIR}/EntryPoint.o
            -Wl,--start-group
            ${CMAKE_CURRENT_BINARY_DIR}/libK64_Core.a
            ${CMAKE_CURRENT_BINARY_DIR}/Console/libConsole.a
            ${CMAKE_CURRENT_BINARY_DIR}/Keyboard/libKeyboard.a
            -Wl,--end-group
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/EntryPoint.o K64_Core Console Keyboard
    COMMENT "Linking Kernel64.Elf with custom command..."
)

# 3) 최종 Kernel64.bin 바이너리 추출
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Kernel64.bin
    COMMAND ${OC64} -j .text -j .data -j .rodata -j .bss -S -O binary ${CMAKE_CURRENT_BINARY_DIR}/Kernel64.Elf ${CMAKE_CURRENT_BINARY_DIR}/Kernel64.bin
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Kernel64.Elf
    COMMENT "Generating final Kernel64.bin..."
)

add_custom_target(Kernel64 DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Kernel64.bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
