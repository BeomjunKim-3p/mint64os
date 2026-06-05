<<<<<<< Updated upstream
set(MODULE_SOURCES
	Private/Keyboard.c
)

set(MODULE_HEADERS
	Public/Keyboard.h
)

add_library(Keyboard STATIC
	${MODULE_SOURCES}
	${MODULE_HEADERS}
)

target_include_directories(Keyboard
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/Public

	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(Keyboard
	PUBLIC
		K64_Core
)

target_compile_options(Keyboard PRIVATE 
    -ffreestanding 
    -nostdlib 
    -fno-rtti 
    -fno-exceptions
)
=======
set(MODULE_SOURCES
	Private/Keyboard.c
)

set(MODULE_HEADERS
	Public/Keyboard.h
)

add_library(Keyboard STATIC
	${MODULE_SOURCES}
	${MODULE_HEADERS}
)

target_include_directories(Keyboard
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/Public

	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(Keyboard
)

target_compile_options(Keyboard PRIVATE 
    -ffreestanding 
    -nostdlib 
    -fno-rtti 
    -fno-exceptions
)
>>>>>>> Stashed changes
