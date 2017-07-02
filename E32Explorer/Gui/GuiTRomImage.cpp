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

bool GuiTRomImage::render() {
	if (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowTitleAlign.x = 0.5;

		if (show_header_window) {
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Always);
			ImGui::SetNextWindowSize(ImVec2(390, 510), ImGuiSetCond_Always);
			ImGui::Begin("Header", &show_header_window);
			render_header_window(image.header);
			ImGui::End();
		}

		if (show_metadata_window) {
			ImGui::SetNextWindowPos(ImVec2(10, 530), ImGuiSetCond_Always);
			ImGui::SetNextWindowSize(ImVec2(240, 240), ImGuiSetCond_Always);
			ImGui::Begin("Flags", &show_metadata_window);
			render_flags_window(image.header);
			ImGui::End();
		}

		if (show_export_dir_window) {
			ImGui::SetNextWindowPos(ImVec2(410, 10), ImGuiSetCond_Always);
			ImGui::SetNextWindowSize(ImVec2(150, 510), ImGuiSetCond_Always);
			ImGui::Begin("Export Dir", &show_export_dir_window);
			render_export_windows(image);
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



void GuiTRomImage::render_header_window(TRomImageHeader& header) {
	ImGui::Columns(2, "File Infos", true);
	ImGui::SetColumnOffset(1, 235);

	ImGui::Selectable("uid1"); ImGui::NextColumn(); imgui_print_hex(header.uid1); ImGui::NextColumn();
	ImGui::Selectable("uid2"); ImGui::NextColumn(); imgui_print_hex(header.uid2); ImGui::NextColumn();
	ImGui::Selectable("uid3"); ImGui::NextColumn(); imgui_print_hex(header.uid3); ImGui::NextColumn();
	ImGui::Selectable("uid_checksum"); ImGui::NextColumn(); imgui_print_hex(header.uid_checksum);
	if (image.valid_uid_checksum) { ImGui::SameLine(); ImGui::Selectable("(Valid)"); }
	else { ImGui::SameLine(); ImGui::Selectable("(Invalid)"); }
	ImGui::NextColumn();
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

void GuiTRomImage::render_flags_window(TRomImageHeader& header) {
			ImGui::Columns(2, "FlagsTable", true);
			ImGui::SetColumnOffset(1, 150);
			
			ImGui::Selectable("Executable type:"); ImGui::NextColumn();
			ImGui::Selectable(header.flags.executable_type ? "DLL" : "Exe"); ImGui::NextColumn();

			ImGui::Selectable("Fixed address:"); ImGui::NextColumn();
			ImGui::Selectable(header.flags.fixed_address ? "Yes" : "No"); ImGui::NextColumn();

			ImGui::Selectable("ABI:"); ImGui::NextColumn();
			switch (header.flags.abi) {
			case 0: ImGui::Selectable("GCC98r2"); break;
			case 1: ImGui::Selectable("EABI"); break;
			default: ImGui::Selectable("Unknown"); break;
			}
			ImGui::NextColumn();
			
			ImGui::Selectable("Entry point type:"); ImGui::NextColumn();
			switch (header.flags.entry_point_type) {
			case 0: ImGui::Selectable("EKA1"); break;
			case 1: ImGui::Selectable("EKA2"); break;
			default: ImGui::Selectable("Unknown"); break;
			}
			ImGui::NextColumn();

			ImGui::Selectable("Exe in tree:"); ImGui::NextColumn();
			ImGui::Selectable(header.flags.exe_in_tree_flag ? "Yes" : "No"); ImGui::NextColumn();

			ImGui::Selectable("Data present:"); ImGui::NextColumn();
			ImGui::Selectable(header.flags.data_present_flag ? "Yes" : "No"); ImGui::NextColumn();

			ImGui::Selectable("Data init:"); ImGui::NextColumn();
			ImGui::Selectable(header.flags.data_init_flag ? "Yes" : "No"); ImGui::NextColumn();

			ImGui::Selectable("Data:"); ImGui::NextColumn();
			ImGui::Selectable(header.flags.data_flag ? "Yes" : "No"); ImGui::NextColumn();

			ImGui::Selectable("File server:"); ImGui::NextColumn();
			ImGui::Selectable(header.flags.secondary_flag ? "Yes" : "No"); ImGui::NextColumn();

			ImGui::Selectable("Device driver:"); ImGui::NextColumn();
			ImGui::Selectable(header.flags.device_flag ? "Yes" : "No"); ImGui::NextColumn();

			ImGui::Selectable("Kernel extension:"); ImGui::NextColumn();
			ImGui::Selectable(header.flags.primary_flag ? "Yes" : "No"); ImGui::NextColumn();

}

void GuiTRomImage::render_export_windows(TRomImage& image) {
	ImGui::Columns(2, "exportDirTable", true);
	ImGui::SetColumnOffset(1, 50);
	for (uint32_t i = 0; i < image.export_directory.size(); i++) {
		ImGui::Selectable(std::to_string(i + 1).c_str());
		ImGui::NextColumn();
		imgui_print_hex(image.export_directory[i]);
		ImGui::NextColumn();
	}
}