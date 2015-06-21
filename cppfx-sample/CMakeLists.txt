include_directories("${CMAKE_CURRENT_BINARY_DIR}/../cppfx/" "${CMAKE_CURRENT_BINARY_DIR}")

if(EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s DISABLE_EXCEPTION_CATCHING=0")
	SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -o cppfx-sample.html")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/sample.xml@sample.xml --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/sample-atlas.png@sample-atlas.png --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/sample-atlas.xml@sample-atlas.xml --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/sample-spritesheet.xml@sample-spritesheet.xml --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/Orbitron.fnt@Orbitron.fnt --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/OrbitronSmall.fnt@OrbitronSmall.fnt")
else()
	set(SDL_BUILDING_LIBRARY FALSE)
	find_package(SDL)

	if(SDL_FOUND)
		set(CPPFX_USE_SDL 1)
		include_directories("${SDL_INCLUDE_DIR}")
		set(DEPENDENCIES ${DEPENDENCIES} ${SDL_LIBRARY})
	endif()

	if(GLEW_FOUND)
		set(CPPFX_USE_GLEW 1)
		if(GLEW_STATIC)
			add_definitions(-DGLEW_STATIC)
		endif()
		include_directories("${GLEW_INCLUDE_DIR}")
	endif()
endif()

add_executable(cppfx-sample 
	"${CMAKE_CURRENT_SOURCE_DIR}/cppfx-sample.cpp"
)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ -std=c++11 ${CMAKE_CXX_FLAGS}")
endif()

target_link_libraries (cppfx-sample LINK_PUBLIC cppfx)

add_custom_command(TARGET cppfx-sample
	POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Arial_30.fnt ${CMAKE_CURRENT_BINARY_DIR}/.
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sample.xml ${CMAKE_CURRENT_BINARY_DIR}/.
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sample-atlas.bmp ${CMAKE_CURRENT_BINARY_DIR}/.
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sample-atlas.xml ${CMAKE_CURRENT_BINARY_DIR}/.
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sample-spritesheet.xml ${CMAKE_CURRENT_BINARY_DIR}/.
)
install(TARGETS cppfx-sample DESTINATION bin)
if(EMSCRIPTEN)
else()
	install(
		FILES 
			"${CMAKE_CURRENT_SOURCE_DIR}/Orbitron.fnt
			${CMAKE_CURRENT_SOURCE_DIR}/OrbitronSmall.fnt
			${CMAKE_CURRENT_SOURCE_DIR}/sample.xml
			${CMAKE_CURRENT_SOURCE_DIR}/sample-atlas.png
			${CMAKE_CURRENT_SOURCE_DIR}/sample-spritesheet.xml"
		DESTINATION bin
	)
endif()