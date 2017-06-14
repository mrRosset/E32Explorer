set(SRCS
		E32Explorer.cpp
		Loader.cpp
		Utils.cpp
		Gui/Gui.cpp
		Gui/GuiE32Image.cpp
		Gui/GuiTRomImage.cpp
		imgui/imgui.cpp
		imgui/imgui_demo.cpp
		imgui/imgui_draw.cpp
		imgui/imgui_impl_glfw_gl3.cpp
		deps/gl3w.c
 	)
	
set(HEADERS
		E32Image.h
		E32Std.h
		Loader.h
		TRomImage.h
		Utils.h
		Gui/Gui.h
		Gui/GuiE32Image.h
		Gui/GuiTRomImage.h
		Gui/MemoryEditor.h
		imgui/imconfig.h
		imgui/imgui.h
		imgui/imgui_impl_glfw_gl3.h
		imgui/imgui_internal.h
		imgui/stb_rect_pack.h
		imgui/stb_textedit.h
		imgui/stb_truetype.h
	)

	
create_directory_groups(${SRCS} ${HEADERS})