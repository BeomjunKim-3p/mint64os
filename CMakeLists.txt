cmake_minimum_required(VERSION 3.10)

# 크로스 컴파일을 위해 컴파일러 테스트 비활성화
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(Mint64OS C ASM_NASM)

set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_DEBUG "-O3 -g")

# NASM이 자동으로 elf64를 사용하지 않도록 컴파일 규칙 오버라이드
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -MD <OBJECT>.d -MT <OBJECT> -o <OBJECT> <SOURCE>")

# --- 도구 경로 설정 (사용자 환경에 맞춰 수정 필요) ---
set(CROSS_COMPILE_PATH "/usr/cross/bin/x86_64-pc-linux-")
set(CC32 "gcc")
set(LD32 "ld")
set(OC32 "objcopy")

set(CC64 "gcc")
set(LD64 "ld")
set(OC64 "objcopy")

# --- ImageMaker 빌드 (네이티브 호스트 컴파일) ---
# ImageMaker는 빌드 시간에만 호스트에서 실행되는 유틸리티
set(_IMAGEMAKER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/04.Utility/00.ImageMaker/ImageMaker.c)
set(_IMAGEMAKER_BIN ${CMAKE_BINARY_DIR}/Utility/ImageMaker.elf)
add_custom_command(
    OUTPUT ${_IMAGEMAKER_BIN}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/Utility
    COMMAND gcc -o ${_IMAGEMAKER_BIN} ${_IMAGEMAKER_SRC}
    DEPENDS ${_IMAGEMAKER_SRC}
    COMMENT "Building ImageMaker.elf (native host tool)"
    VERBATIM
)
add_custom_target(ImageMaker.elf_target ALL DEPENDS ${_IMAGEMAKER_BIN})

# 서브 디렉토리 추가
add_subdirectory(04.Utility)
add_subdirectory(00.BootLoader)
add_subdirectory(01.Kernel32)
add_subdirectory(02.kernel64)

enable_language(ASM_NASM)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/Disk.img
    COMMAND ${_IMAGEMAKER_BIN}
            ${CMAKE_BINARY_DIR}/00.BootLoader/BootLoader.bin
            ${CMAKE_BINARY_DIR}/01.Kernel32/Kernel32.bin 
            ${CMAKE_BINARY_DIR}/02.kernel64/Kernel64.bin
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${_IMAGEMAKER_BIN} BootLoader Kernel32 Kernel64
    COMMENT "Creating final Disk.img using ImageMaker"
)   

add_custom_target(DiskImage ALL DEPENDS ${CMAKE_BINARY_DIR}/Disk.img)

