#include "GuiTRomImage.h"

#include <type_traits> 

GuiTRomImage::GuiTRomImage(TRomImage& image_, std::string additional_title) :
	Gui(additional_title),
	image(image_)
{

}

template<typename E>
constexpr auto to_underlying(E e) -> typename std::underlying_type<E>::type
{
	return static_cast<typename std::underlying_type<E>::type>(e);
}

void GuiTRomImage::render_header_window(TRomImageHeader& header) {
	ImGui::Columns(2, "File Infos", true);
	ImGui::Selectable("uid1"); ImGui::NextColumn(); imgui_print_hex(header.uid1); ImGui::NextColumn();
	ImGui::Selectable("uid2"); ImGui::NextColumn(); imgui_print_hex(header.uid2); ImGui::NextColumn();
	ImGui::Selectable("uid3"); ImGui::NextColumn(); imgui_print_hex(header.uid3); ImGui::NextColumn();
	ImGui::Selectable("uid_checksum"); ImGui::NextColumn(); imgui_print_hex(header.uid_checksum); ImGui::NextColumn();
	ImGui::Selectable("entry_point"); ImGui::NextColumn(); imgui_print_hex(header.entry_point); ImGui::NextColumn();
	ImGui::Selectable("code_address"); ImGui::NextColumn(); imgui_print_hex(header.code_address); ImGui::NextColumn();
	ImGui::Selectable("data_address"); ImGui::NextColumn(); imgui_print_hex(header.data_address); ImGui::NextColumn();
	ImGui::Selectable("code_size"); ImGui::NextColumn(); imgui_print_hex(header.code_size); ImGui::NextColumn();
	ImGui::Selectable("text_size"); ImGui::NextColumn(); imgui_print_hex(header.text_size); ImGui::NextColumn();
	ImGui::Selectable("data_size"); ImGui::NextColumn(); imgui_print_hex(header.data_size); ImGui::NextColumn();
	ImGui::Selectable("bss_size"); ImGui::NextColumn(); imgui_print_hex(header.bss_size); ImGui::NextColumn();
	ImGui::Selectable("heap_minimum_size"); ImGui::NextColumn(); imgui_print_hex(header.heap_minimum_size); ImGui::NextColumn();
	ImGui::Selectable("heap_maximum_size"); ImGui::NextColumn(); imgui_print_hex(header.heap_maximum_size); ImGui::NextColumn();
	ImGui::Selectable("stack_size"); ImGui::NextColumn(); imgui_print_hex(header.stack_size); ImGui::NextColumn();
	ImGui::Selectable("dll_ref_table_address"); ImGui::NextColumn(); imgui_print_hex(header.dll_ref_table_address); ImGui::NextColumn();
	ImGui::Selectable("export_dir_count"); ImGui::NextColumn(); imgui_print_hex(header.export_dir_count); ImGui::NextColumn();
	ImGui::Selectable("export_dir_address"); ImGui::NextColumn(); imgui_print_hex(header.export_dir_address); ImGui::NextColumn();
	ImGui::Selectable("code_checksum"); ImGui::NextColumn(); imgui_print_hex(header.code_checksum); ImGui::NextColumn();
	ImGui::Selectable("data_checksum"); ImGui::NextColumn(); imgui_print_hex(header.data_checksum); ImGui::NextColumn();
	ImGui::Selectable("major"); ImGui::NextColumn(); imgui_print_hex(header.major); ImGui::NextColumn();
	ImGui::Selectable("minor"); ImGui::NextColumn(); imgui_print_hex(header.minor); ImGui::NextColumn();
	ImGui::Selectable("build"); ImGui::NextColumn(); imgui_print_hex(header.build); ImGui::NextColumn();
	ImGui::Selectable("flags_raw"); ImGui::NextColumn(); imgui_print_hex(header.flags_raw); ImGui::NextColumn();
	ImGui::Selectable("priority"); ImGui::NextColumn(); imgui_print_hex(to_underlying(header.priority));
	ImGui::SameLine();
	switch (header.priority) {
	case ProcessPriority::Background: ImGui::Selectable("(Background)"); break;
	case ProcessPriority::FileServer: ImGui::Selectable("(FileServer)"); break;
	case ProcessPriority::Foreground: ImGui::Selectable("(Foreground)"); break;
	case ProcessPriority::High: ImGui::Selectable("(High)"); break;
	case ProcessPriority::Low: ImGui::Selectable("(Low)"); break;
	case ProcessPriority::RealTimeServer: ImGui::Selectable("(RealTimeServer)"); break;
	case ProcessPriority::Supervisor: ImGui::Selectable("(Supervisor)"); break;
	case ProcessPriority::WindowServer: ImGui::Selectable("(WindowServer)"); break;
	default: ImGui::Selectable("(Unknown)"); break;
	}
	ImGui::NextColumn();
	ImGui::Selectable("data_bss_linear_base_address"); ImGui::NextColumn(); imgui_print_hex(header.data_bss_linear_base_address); ImGui::NextColumn();
	ImGui::Selectable("next_extension_linear_address"); ImGui::NextColumn(); imgui_print_hex(header.next_extension_linear_address); ImGui::NextColumn();
	ImGui::Selectable("harware_variant"); ImGui::NextColumn(); imgui_print_hex(header.harware_variant); ImGui::NextColumn();
}


bool GuiTRomImage::render() {
	if (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowTitleAlign.x = 0.5;

		if (show_header_window) {
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(335, 600), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Header", &show_header_window);
			render_header_window(image.header);
			ImGui::End();
		}
		//ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		//ImGui::ShowTestWindow(&show_test_window);

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		glfwSwapBuffers(window);

		return true;
	}
	else {
		return false;
	}
}
