set(CC32ARG "-c;-m32;-ffreestanding;-g;-nostdlib")

set(CC32 "${CC32};${CC32ARG}")
set(LD32 "${LD32};-melf_i386;-T;${CMAKE_CURRENT_SOURCE_DIR}/elf_i386.x;-nostdlib;-e;Main;-Ttext;0x10400")
set(OC32 "${OC32};-j;.text;-j;.data;-j;.rodata;-j;.bss;-S;-O;binary")

# 임시 오브젝트 및 바이너리 저장 경로
set(K32_TEMP ${CMAKE_CURRENT_BINARY_DIR}/Temp)
file(MAKE_DIRECTORY ${K32_TEMP})

make_directory(${K32_TEMP})

# 1. EntryPoint.s 빌드
add_custom_command(
    OUTPUT ${K32_TEMP}/EntryPoint.bin
    COMMAND nasm -o ${K32_TEMP}/EntryPoint.bin ${CMAKE_CURRENT_SOURCE_DIR}/Source/EntryPoint.s
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Source/EntryPoint.s
    COMMENT "Building EntryPoint.bin (Raw Binary)"
)


# 2. 소스 파일 수집
file(GLOB_RECURSE K32_CSOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.c")
file(GLOB_RECURSE K32_ASOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.asm")

# 3. 커널 오브젝트 빌드

add_library(K32_Objs OBJECT ${K32_CSOURCES} ${K32_ASOURCES})

# C 파일 및 ASM 파일에 대한 세부 옵션 설정

target_compile_options(K32_Objs PRIVATE 
	$<$<COMPILE_LANGUAGE:C>:${CC32ARG}>
	$<$<COMPILE_LANGUAGE:ASM_NASM>:-f;elf32>
)


# 4. 링크 및 바이너리 최종 결합
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Kernel32.bin
    COMMAND ${LD32} -o ${K32_TEMP}/Kernel32.elf $<TARGET_OBJECTS:K32_Objs>
    COMMAND ${OC32}  ${K32_TEMP}/Kernel32.elf ${K32_TEMP}/Kernel32.elf.bin
    COMMAND cat ${K32_TEMP}/EntryPoint.bin ${K32_TEMP}/Kernel32.elf.bin > ${CMAKE_CURRENT_BINARY_DIR}/Kernel32.bin
    DEPENDS ${K32_TEMP}/EntryPoint.bin K32_Objs
    COMMENT "Linking Kernel32.elf and creating Kernel32.bin"
    COMMAND_EXPAND_LISTS
    VERBATIM
)

# 최종 타겟 설정
add_custom_target(Kernel32 ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Kernel32.bin)
