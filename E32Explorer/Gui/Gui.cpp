#include "Gui.h"
#include <string>
#include <iostream>
#include <sstream>
#include <inttypes.h>
#include <type_traits> 

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

Gui::Gui(std::string additional_title) {
	// Setup window
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	window = glfwCreateWindow(1280, 780, ("E32Explorer - " + additional_title).c_str(), NULL, NULL);
	//center windows
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int window_width, window_height;
	glfwGetWindowSize(window, &window_width, &window_height);
	glfwSetWindowPos(window, (mode->width - window_width)/2, (mode->height - window_height) / 2);

	glfwMakeContextCurrent(window);
	gl3wInit();

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);
}

Gui::~Gui() {
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}

void Gui::imgui_print_hex(uint8_t u) {
	char label[32];
	sprintf_s(label, "%02X", u);
	ImGui::Selectable(label);
}

void Gui::imgui_print_hex(uint16_t u) {
	char label[32];
	sprintf_s(label, "%08X", u);
	ImGui::Selectable(label);
}

void Gui::imgui_print_hex(uint32_t u) {
	char label[32];
	sprintf_s(label, "%08X", u);
	ImGui::Selectable(label);
}

void Gui::imgui_print_hex(int32_t i) {
	char label[32];
	sprintf_s(label, "%08X", i);
	ImGui::Selectable(label);
}

void Gui::imgui_print_hex(int64_t i) {
	char label[32];
	sprintf_s(label, "%00X%" PRId64 "", i);
	ImGui::Selectable(label);
}