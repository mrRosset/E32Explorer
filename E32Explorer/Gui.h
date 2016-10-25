#pragma once

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <cstdlib>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "E32Image.h"
#include "MemoryEditor.h"

class Gui {
private:
	GLFWwindow* window;
	bool show_test_window = true;
	bool show_header_window = true;
	bool show_sections_window = true;
	bool show_metadata_window = true;
	bool show_iat_window = true;
	bool show_export_dir_window = true;
	MemoryEditor memory_editor;
	ImVec4 clear_color = ImColor(114, 144, 154);

public:
	Gui();
	~Gui();
	bool render(E32Image& image);

};