cmake_minimum_required (VERSION 3.0)
project (E32Explorer)

# Visual Studio specific
set(CMAKE_SUPPRESS_REGENERATION true) # stop creating ZERO_CHECK project
add_definitions(-D_CRT_SECURE_NO_WARNINGS) # stop complaining about sprintf_s and sscanf_s

set(CMAKE_CXX_FLAGS "-std=c++0x")

include_directories (${PROJECT_SOURCE_DIR}/deps/include/)
file(GLOB sources
		${PROJECT_SOURCE_DIR}/E32Explorer/*.cpp
		${PROJECT_SOURCE_DIR}/E32Explorer/imgui/*.cpp
		${PROJECT_SOURCE_DIR}/E32Explorer/Gui/*.cpp
		${PROJECT_SOURCE_DIR}/deps/gl3w.c
	)
add_executable(${PROJECT_NAME} ${sources})


find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS} ${OPENGL_gl_LIBRARY})


set(vs_arch ${CMAKE_VS_PLATFORM_NAME})
if (vs_arch STREQUAL Win32)
	add_library(glfw STATIC IMPORTED)
	set_target_properties(glfw PROPERTIES IMPORTED_LOCATION
		${PROJECT_SOURCE_DIR}/deps/lib32/glfw3.lib)
elseif (vs_arch STREQUAL x64)
	add_library(glfw STATIC IMPORTED)
	set_target_properties(glfw PROPERTIES IMPORTED_LOCATION
		${PROJECT_SOURCE_DIR}/deps/lib64/glfw3.lib)
else()
	find_package(glfw3 REQUIRED)
endif()
target_link_libraries(${PROJECT_NAME} glfw)
