cmake_minimum_required (VERSION 3.1)
project (E32Explorer)

# Visual Studio specific
set(CMAKE_SUPPRESS_REGENERATION true) # stop creating ZERO_CHECK project
add_definitions(-D_CRT_SECURE_NO_WARNINGS) # stop complaining about sprintf_s and sscanf_s

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #avoid using compiler-specific extension (avoid using gnu++11)

function(create_directory_groups)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    foreach(file_name ${ARGV})
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

include_directories (${PROJECT_SOURCE_DIR}/deps/include/)

add_subdirectory(E32Explorer)

#file(GLOB SRCS
		# E32Explorer/*.cpp
 		# E32Explorer/imgui/*.cpp
		# E32Explorer/Gui/*.cpp
		# E32Explorer/Loader/*.cpp
		# deps/gl3w.c
 	# )
	
#file(GLOB HEADERS
		# E32Explorer/*.h
 		# E32Explorer/imgui/*.h
		# E32Explorer/Gui/*.h
		# E32Explorer/Loader/*.h
	# )
	
set(SRCS
		E32Explorer/E32Explorer.cpp
		E32Explorer/Utils.cpp
		E32Explorer/Loader/E32ImageLoader.cpp
		E32Explorer/Loader/TRomImageLoader.cpp
		E32Explorer/Loader/TRomLoader.cpp
		E32Explorer/Gui/Gui.cpp
		E32Explorer/Gui/GuiE32Image.cpp
		E32Explorer/Gui/GuiTRomImage.cpp
		E32Explorer/imgui/imgui.cpp
		E32Explorer/imgui/imgui_demo.cpp
		E32Explorer/imgui/imgui_draw.cpp
		E32Explorer/imgui/imgui_impl_glfw_gl3.cpp
		deps/gl3w.c
 	)
	
set(HEADERS
		E32Explorer/E32Image.h
		E32Explorer/E32Std.h
		E32Explorer/TRomImage.h
		E32Explorer/TRom.h
		E32Explorer/Utils.h
		E32Explorer/Loader/E32ImageLoader.h
		E32Explorer/Loader/TRomImageLoader.h
		E32Explorer/Loader/TRomLoader.h
		E32Explorer/Gui/Gui.h
		E32Explorer/Gui/GuiE32Image.h
		E32Explorer/Gui/GuiTRomImage.h
		E32Explorer/Gui/MemoryEditor.h
		E32Explorer/imgui/imconfig.h
		E32Explorer/imgui/imgui.h
		E32Explorer/imgui/imgui_impl_glfw_gl3.h
		E32Explorer/imgui/imgui_internal.h
		E32Explorer/imgui/stb_rect_pack.h
		E32Explorer/imgui/stb_textedit.h
		E32Explorer/imgui/stb_truetype.h
	)
	
create_directory_groups(${SRCS} ${HEADERS})

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})

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
