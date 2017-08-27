#include "GuiTRom.h"

#include <type_traits> 

GuiTRom::GuiTRom(TRom& image_, std::string additional_title) :
	Gui(additional_title),
	image(image_) {}

template<typename E>
constexpr auto to_underlying(E e) -> typename std::underlying_type<E>::type
{
	return static_cast<typename std::underlying_type<E>::type>(e);
}

bool GuiTRom::render() {
	if (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowTitleAlign.x = 0.5;

		if (show_header_window) {
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_Always);
			ImGui::SetNextWindowSize(ImVec2(350, 510), ImGuiSetCond_Always);
			ImGui::Begin("Header", &show_header_window);
			render_header_window();
			ImGui::End();
		}

		if (show_dir_list_window) {
			ImGui::SetNextWindowPos(ImVec2(10, 530), ImGuiSetCond_Always);
			ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiSetCond_Always);
			ImGui::Begin("DirList", &show_dir_list_window);
			render_dir_list_window();
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

void GuiTRom::render_header_window() {
	TRomHeader& header = image.header;
	ImGui::Columns(2, "Rom Infos", true);
	ImGui::SetColumnOffset(1, 235);

	ImGui::Selectable("major"); ImGui::NextColumn(); imgui_print_hex(header.major); ImGui::NextColumn();
	ImGui::Selectable("minor"); ImGui::NextColumn(); imgui_print_hex(header.minor); ImGui::NextColumn();
	ImGui::Selectable("build"); ImGui::NextColumn(); imgui_print_hex(header.build); ImGui::NextColumn();
	ImGui::Selectable("time"); ImGui::NextColumn(); imgui_print_hex(header.time); ImGui::NextColumn();
	ImGui::Selectable("rom_base"); ImGui::NextColumn(); imgui_print_hex(header.rom_base_address); ImGui::NextColumn();
	ImGui::Selectable("rom_size"); ImGui::NextColumn(); imgui_print_hex(header.rom_size); ImGui::NextColumn();
	ImGui::Selectable("rom_root_directory_list"); ImGui::NextColumn(); imgui_print_hex(header.rom_root_directory_list); ImGui::NextColumn();
	ImGui::Selectable("kern_data_address"); ImGui::NextColumn(); imgui_print_hex(header.kern_data_address); ImGui::NextColumn();
	ImGui::Selectable("kern_stack_address"); ImGui::NextColumn(); imgui_print_hex(header.kern_stack_address); ImGui::NextColumn();
	ImGui::Selectable("primary_file"); ImGui::NextColumn(); imgui_print_hex(header.primary_file); ImGui::NextColumn();
	ImGui::Selectable("secondary_file"); ImGui::NextColumn(); imgui_print_hex(header.secondary_file); ImGui::NextColumn();
	ImGui::Selectable("checksum"); ImGui::NextColumn(); imgui_print_hex(header.checksum); ImGui::NextColumn();
	ImGui::Selectable("language"); ImGui::NextColumn(); imgui_print_hex(header.language); ImGui::NextColumn();
	ImGui::Selectable("hardware"); ImGui::NextColumn(); imgui_print_hex(header.hardware); ImGui::NextColumn();
	ImGui::Selectable("screen_width"); ImGui::NextColumn(); imgui_print_hex(header.screen_width); ImGui::NextColumn();
	ImGui::Selectable("screen_height"); ImGui::NextColumn(); imgui_print_hex(header.screen_height); ImGui::NextColumn();
	ImGui::Selectable("screen_bits_per_pixel"); ImGui::NextColumn(); imgui_print_hex(header.screen_bits_per_pixel); ImGui::NextColumn();
	ImGui::Selectable("rom_section_header"); ImGui::NextColumn(); imgui_print_hex(header.rom_section_header); ImGui::NextColumn();
	ImGui::Selectable("total_sv_data_size"); ImGui::NextColumn(); imgui_print_hex(header.total_sv_data_size); ImGui::NextColumn();
	ImGui::Selectable("variant_file"); ImGui::NextColumn(); imgui_print_hex(header.variant_file); ImGui::NextColumn();
	ImGui::Selectable("extension_file"); ImGui::NextColumn(); imgui_print_hex(header.extension_file); ImGui::NextColumn();
	ImGui::Selectable("reloc_info"); ImGui::NextColumn(); imgui_print_hex(header.reloc_info); ImGui::NextColumn();
	ImGui::Selectable("trace_mask"); ImGui::NextColumn(); imgui_print_hex(header.trace_mask); ImGui::NextColumn();
	ImGui::Selectable("user_data_address"); ImGui::NextColumn(); imgui_print_hex(header.user_data_address); ImGui::NextColumn();
	ImGui::Selectable("total_user_data_size"); ImGui::NextColumn(); imgui_print_hex(header.total_user_data_size); ImGui::NextColumn();
}

void GuiTRom::render_dir_list_window() {
	auto& dir_list = image.directory_list;
	ImGui::Columns(2, "DirList", true);
	//ImGui::SetColumnOffset(1, 235);
	ImGui::Selectable("Number Root Dir"); ImGui::NextColumn(); imgui_print_hex(dir_list.num_root_dir); ImGui::NextColumn();
	ImGui::Selectable("1:"); ImGui::NextColumn(); ImGui::NextColumn();
	ImGui::Selectable("Hardware Variant"); ImGui::NextColumn(); imgui_print_hex(dir_list.root_dir.hardware_variant); ImGui::NextColumn();
	ImGui::Selectable("Address"); ImGui::NextColumn(); imgui_print_hex(dir_list.root_dir.address_lin); ImGui::NextColumn();

}

