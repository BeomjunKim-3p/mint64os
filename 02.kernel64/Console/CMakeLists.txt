set(MODULE_SOURCES
	Private/ConsoleManager.c
)

set(MODULE_HEADERS
	Public/Console.h
)

add_library(Console STATIC 
	${MODULE_SOURCES}
	${MODULE_HEADERS}
)

target_include_directories(Console
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/Public

	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(Console
	PUBLIC
		K64_Core
	PRIVATE
		Keyboard
)

target_compile_options(Console PRIVATE 
    -ffreestanding 
    -nostdlib 
    -fno-rtti 
    -fno-exceptions
)
