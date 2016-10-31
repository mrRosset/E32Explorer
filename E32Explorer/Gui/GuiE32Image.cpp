#include "GuiE32Image.h"

#include <type_traits> 

GuiE32Image::GuiE32Image(E32Image& image_, std::string additional_title):
	Gui(additional_title),
	image(image_)
{

}

template<typename E>
constexpr auto to_underlying(E e) -> typename std::underlying_type<E>::type
{
	return static_cast<typename std::underlying_type<E>::type>(e);
}

bool GuiE32Image::render() {
	if (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		E32ImageHeader& header = image.header;

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowTitleAlign.x = 0.5;

		if (show_header_window) {
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(335, 600), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Header", &show_header_window);
			ImGui::Columns(2, "File Infos", true);
			ImGui::Selectable("uid1"); ImGui::NextColumn(); imgui_print_hex(header.uid1); ImGui::NextColumn();
			ImGui::Selectable("uid2"); ImGui::NextColumn(); imgui_print_hex(header.uid2); ImGui::NextColumn();
			ImGui::Selectable("uid3"); ImGui::NextColumn(); imgui_print_hex(header.uid3); ImGui::NextColumn();
			ImGui::Selectable("uid_checksum"); ImGui::NextColumn();
			imgui_print_hex(header.uid_checksum);
			if (image.valid_uid_checksum) { ImGui::SameLine(); ImGui::Selectable("(Valid)"); }
			ImGui::NextColumn();
			ImGui::Selectable("signature"); ImGui::NextColumn(); imgui_print_hex(header.signature);
			ImGui::SameLine();
			char signature_text[] = { '(', header.signature & 0xFF, (header.signature >> 8) & 0xFF, (header.signature >> 16) & 0xFF, (header.signature >> 24) & 0xFF, ')' ,'\0' };
			ImGui::Selectable(signature_text);
			ImGui::NextColumn();
			ImGui::Selectable("cpu"); ImGui::NextColumn(); imgui_print_hex(to_underlying(header.cpu));
			ImGui::SameLine();
			switch (header.cpu) {
			case CPUType::x86: ImGui::Selectable("(x86)"); break;
			case CPUType::Arm: ImGui::Selectable("(Arm)"); break;
			case CPUType::Mcore: ImGui::Selectable("(M*core)"); break;
			default: ImGui::Selectable("(Unknown)"); break;
			}
			ImGui::NextColumn();
			ImGui::Selectable("code_checksum"); ImGui::NextColumn(); imgui_print_hex(header.code_checksum); ImGui::NextColumn();
			ImGui::Selectable("data_checksum"); ImGui::NextColumn(); imgui_print_hex(header.data_checksum); ImGui::NextColumn();
			ImGui::Selectable("major"); ImGui::NextColumn(); imgui_print_hex(header.major); ImGui::NextColumn();
			ImGui::Selectable("minor"); ImGui::NextColumn(); imgui_print_hex(header.minor); ImGui::NextColumn();
			ImGui::Selectable("build"); ImGui::NextColumn(); imgui_print_hex(header.build); ImGui::NextColumn();
			ImGui::Selectable("timestamp"); ImGui::NextColumn(); imgui_print_hex(header.timestamp); ImGui::NextColumn();
			ImGui::Selectable("flags"); ImGui::NextColumn(); imgui_print_hex(header.flags_raw); ImGui::NextColumn();
			ImGui::Selectable("code_size"); ImGui::NextColumn(); imgui_print_hex(header.code_size); ImGui::NextColumn();
			ImGui::Selectable("data_size"); ImGui::NextColumn(); imgui_print_hex(header.data_size); ImGui::NextColumn();
			ImGui::Selectable("heap_minimum_size"); ImGui::NextColumn(); imgui_print_hex(header.heap_minimum_size); ImGui::NextColumn();
			ImGui::Selectable("heap_maximum_size"); ImGui::NextColumn(); imgui_print_hex(header.heap_maximum_size); ImGui::NextColumn();
			ImGui::Selectable("stack_size"); ImGui::NextColumn(); imgui_print_hex(header.stack_size); ImGui::NextColumn();
			ImGui::Selectable("BSS_size"); ImGui::NextColumn(); imgui_print_hex(header.BSS_size); ImGui::NextColumn();
			ImGui::Selectable("entry_point_offset"); ImGui::NextColumn(); imgui_print_hex(header.entry_point_offset); ImGui::NextColumn();
			ImGui::Selectable("code_base_address"); ImGui::NextColumn(); imgui_print_hex(header.code_base_address); ImGui::NextColumn();
			ImGui::Selectable("data_base_address"); ImGui::NextColumn(); imgui_print_hex(header.data_base_address); ImGui::NextColumn();
			ImGui::Selectable("dll_count"); ImGui::NextColumn(); imgui_print_hex(header.dll_count); ImGui::NextColumn();
			ImGui::Selectable("export_offset"); ImGui::NextColumn(); imgui_print_hex(header.export_offset); ImGui::NextColumn();
			ImGui::Selectable("export_count"); ImGui::NextColumn(); imgui_print_hex(header.export_count); ImGui::NextColumn();
			ImGui::Selectable("text_size"); ImGui::NextColumn(); imgui_print_hex(header.text_size); ImGui::NextColumn();
			ImGui::Selectable("code_offset"); ImGui::NextColumn(); imgui_print_hex(header.code_offset); ImGui::NextColumn();
			ImGui::Selectable("data_offset"); ImGui::NextColumn(); imgui_print_hex(header.data_offset); ImGui::NextColumn();
			ImGui::Selectable("import_offset"); ImGui::NextColumn(); imgui_print_hex(header.import_offset); ImGui::NextColumn();
			ImGui::Selectable("code_relocation_offset"); ImGui::NextColumn(); imgui_print_hex(header.code_relocation_offset); ImGui::NextColumn();
			ImGui::Selectable("data_relocation_offset"); ImGui::NextColumn(); imgui_print_hex(header.data_relocation_offset); ImGui::NextColumn();
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
			ImGui::Columns(1, "End text");
			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (show_sections_window) {
			ImGui::SetNextWindowPos(ImVec2(10, 620), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(580, 150), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Sections", &show_sections_window);
			ImGui::Columns(6, "SectionTable", true);
			ImGui::SetColumnOffset(1, 70);
			ImGui::SetColumnOffset(2, 150);
			ImGui::SetColumnOffset(3, 230);
			ImGui::SetColumnOffset(4, 310);
			ImGui::SetColumnOffset(5, 400);
			ImGui::NextColumn();
			ImGui::Selectable("Offset"); ImGui::NextColumn(); ImGui::Selectable("Size"); ImGui::NextColumn();
			ImGui::Selectable("Relocs"); ImGui::NextColumn(); ImGui::Selectable("NumOfRelocs"); ImGui::NextColumn();
			ImGui::NextColumn();

			ImGui::Selectable("Code"); ImGui::NextColumn();
			imgui_print_hex(header.code_offset); ImGui::NextColumn();
			imgui_print_hex(header.code_size); ImGui::NextColumn();
			if (header.code_relocation_offset) { imgui_print_hex(header.code_relocation_offset); }	ImGui::NextColumn();
			if (header.code_relocation_offset) { imgui_print_hex(image.code_reloc_section.number_of_relocs); } ImGui::NextColumn();
			ImGui::Selectable("Entry point: "); ImGui::SameLine(); imgui_print_hex(header.entry_point_offset); ImGui::NextColumn();

			ImGui::Selectable("Data"); ImGui::NextColumn();
			imgui_print_hex(header.data_offset); ImGui::NextColumn();
			imgui_print_hex(header.data_size); ImGui::NextColumn();
			if (header.data_relocation_offset) { imgui_print_hex(header.data_relocation_offset); } ImGui::NextColumn();
			if (header.data_relocation_offset) { imgui_print_hex(image.data_reloc_section.number_of_relocs); } ImGui::NextColumn();
			ImGui::NextColumn();

			ImGui::Selectable("BSS"); ImGui::NextColumn(); ImGui::NextColumn();
			imgui_print_hex(header.BSS_size); ImGui::NextColumn();
			ImGui::NextColumn(); ImGui::NextColumn(); ImGui::NextColumn();

			ImGui::Selectable("Exports"); ImGui::NextColumn();
			imgui_print_hex(header.export_offset); ImGui::NextColumn();
			imgui_print_hex(header.export_count * 4); ImGui::NextColumn(); //Why *4 ?? Taken from petran pe_dump.cpp
			ImGui::NextColumn(); ImGui::NextColumn();
			ImGui::Selectable(std::to_string(header.export_count).c_str()); ImGui::SameLine(); ImGui::Selectable("Entries"); ImGui::NextColumn();

			ImGui::Selectable("Import"); ImGui::NextColumn();
			imgui_print_hex(header.import_offset); ImGui::NextColumn(); ImGui::NextColumn();
			ImGui::NextColumn(); ImGui::NextColumn(); ImGui::NextColumn();

			ImGui::End();
		}

		if (show_metadata_window) {
			//
			//Pos=624,619
			//Size = 316, 160
			ImGui::SetNextWindowPos(ImVec2(610, 620), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(316, 160), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Flags", &show_sections_window);
			ImGui::Columns(2, "FlagsTable", true);
			ImGui::Selectable("Executable type:"); ImGui::NextColumn();
			switch (header.flags.executable_type) {
			case 0: ImGui::Selectable("Exe"); break;
			case 1: ImGui::Selectable("DLL"); break;
			default: ImGui::Selectable("Unknown"); break;
			}
			ImGui::NextColumn();
			ImGui::Selectable("Entry point:"); ImGui::NextColumn();
			switch (header.flags.call_entry_point) {
			case 0: ImGui::Selectable("Use it"); break;
			case 1: ImGui::Selectable("Don't use it"); break;
			default: ImGui::Selectable("Unknown"); break;
			}
			ImGui::NextColumn();
			ImGui::Selectable("Fixed address: "); ImGui::NextColumn();
			switch (header.flags.fixed_address) {
			case 0: ImGui::Selectable("No"); break;
			case 1: ImGui::Selectable("Yes"); break;
			default: ImGui::Selectable("Unknown"); break;
			}
			ImGui::NextColumn();
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
			ImGui::Selectable("Header format:"); ImGui::NextColumn();
			switch (header.flags.header_format) {
			case 0: ImGui::Selectable("Basic"); break;
			case 1: ImGui::Selectable("J-format"); break;
			case 2: ImGui::Selectable("V-format"); break;
			default: ImGui::Selectable("Unknown"); break;
			}
			ImGui::NextColumn();
			ImGui::Selectable("Import format:"); ImGui::NextColumn();
			switch (header.flags.import_format) {
			case 0: ImGui::Selectable("Standard PE format"); break;
			case 1: ImGui::Selectable("Elf format"); break;
			case 2: ImGui::Selectable("PE without redundancy"); break;
			default: ImGui::Selectable("Unknow"); break;
			}
			ImGui::NextColumn();
			ImGui::End();
		}
		if (show_iat_window) {
			ImGui::SetNextWindowPos(ImVec2(350, 10), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(70, 600), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("IAT", &show_iat_window);
			for (uint32_t iat_entry : image.code_section.import_address_table) {
				ImGui::Selectable(std::to_string(iat_entry).c_str());
			}
			ImGui::End();
		}

		if (show_export_dir_window) {
			ImGui::SetNextWindowPos(ImVec2(640, 10), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(150, 600), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Export Dir", &show_export_dir_window);
			ImGui::Columns(2, "exportDirTable", true);
			ImGui::SetColumnOffset(1, 50);
			for (uint32_t i = 0; i < image.code_section.export_directory.size(); i++) {
				ImGui::Selectable(std::to_string(i + 1).c_str());
				ImGui::NextColumn();
				imgui_print_hex(image.code_section.export_directory[i]);
				ImGui::NextColumn();
			}
			ImGui::End();
		}

		if (show_import_section) {
			ImGui::SetNextWindowPos(ImVec2(425, 10), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(210, 600), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Import Section", &show_export_dir_window);

			//std::cout << "size: " << image.import_section.size << std::endl;
			for (auto& block : image.import_section.imports) {
				//std::cout << "offset: " << block->dll_name_offset << " " << (char*)&image.data[(header.import_offset + block->dll_name_offset)] << std::endl;
				ImGui::Selectable((char*)&image.data[(header.import_offset + block->dll_name_offset)]);
				for (uint32_t ordinal : block->ordinals) {
					//std::cout << ordinal << std::endl;
					ImGui::Selectable(('\t' + std::to_string(ordinal)).c_str());
				}
			}
			//show_import_section = true;

			ImGui::End();
		}

		ImGui::SetNextWindowPos(ImVec2(795
			, 10), ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(478, 475), ImGuiSetCond_FirstUseEver);
		memory_editor.Draw("Code And Constant Section", &image.data[image.header.code_offset], image.header.text_size, image.header.code_offset);



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
