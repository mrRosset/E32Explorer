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
			ImGui::SetNextWindowSize(ImVec2(390, 510), ImGuiSetCond_Always);
			ImGui::Begin("Header", &show_header_window);
			render_header_window();
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
	ImGui::Columns(2, "File Infos", true);
	ImGui::SetColumnOffset(1, 235);

	ImGui::Selectable("major"); ImGui::NextColumn(); imgui_print_hex(header.major); ImGui::NextColumn();
	ImGui::Selectable("minor"); ImGui::NextColumn(); imgui_print_hex(header.minor); ImGui::NextColumn();
	ImGui::Selectable("build"); ImGui::NextColumn(); imgui_print_hex(header.build); ImGui::NextColumn();
	ImGui::Selectable("iTime"); ImGui::NextColumn(); imgui_print_hex(header.iTime); ImGui::NextColumn();
	ImGui::Selectable("iRomBase"); ImGui::NextColumn(); imgui_print_hex(header.iRomBase); ImGui::NextColumn();
	ImGui::Selectable("iRomSize"); ImGui::NextColumn(); imgui_print_hex(header.iRomSize); ImGui::NextColumn();
	ImGui::Selectable("iRomRootDirectoryList"); ImGui::NextColumn(); imgui_print_hex(header.iRomRootDirectoryList); ImGui::NextColumn();
	ImGui::Selectable("iKernDataAddress"); ImGui::NextColumn(); imgui_print_hex(header.iKernDataAddress); ImGui::NextColumn();
	ImGui::Selectable("iKernStackAddress"); ImGui::NextColumn(); imgui_print_hex(header.iKernStackAddress); ImGui::NextColumn();
	ImGui::Selectable("iPrimaryFile"); ImGui::NextColumn(); imgui_print_hex(header.iPrimaryFile); ImGui::NextColumn();
	ImGui::Selectable("iSecondaryFile"); ImGui::NextColumn(); imgui_print_hex(header.iSecondaryFile); ImGui::NextColumn();
	ImGui::Selectable("iCheckSum"); ImGui::NextColumn(); imgui_print_hex(header.iCheckSum); ImGui::NextColumn();
	ImGui::Selectable("iLanguage"); ImGui::NextColumn(); imgui_print_hex(header.iLanguage); ImGui::NextColumn();
	ImGui::Selectable("iHardware"); ImGui::NextColumn(); imgui_print_hex(header.iHardware); ImGui::NextColumn();
	ImGui::Selectable("screen_width"); ImGui::NextColumn(); imgui_print_hex(header.screen_width); ImGui::NextColumn();
	ImGui::Selectable("screen_height"); ImGui::NextColumn(); imgui_print_hex(header.screen_height); ImGui::NextColumn();
	ImGui::Selectable("iScreenBitsPerPixel"); ImGui::NextColumn(); imgui_print_hex(header.iScreenBitsPerPixel); ImGui::NextColumn();
	ImGui::Selectable("iRomSectionHeader"); ImGui::NextColumn(); imgui_print_hex(header.iRomSectionHeader); ImGui::NextColumn();
	ImGui::Selectable("iTotalSvDataSize"); ImGui::NextColumn(); imgui_print_hex(header.iTotalSvDataSize); ImGui::NextColumn();
	ImGui::Selectable("iVariantFile"); ImGui::NextColumn(); imgui_print_hex(header.iVariantFile); ImGui::NextColumn();
	ImGui::Selectable("iExtensionFile"); ImGui::NextColumn(); imgui_print_hex(header.iExtensionFile); ImGui::NextColumn();
	ImGui::Selectable("iRelocInfo"); ImGui::NextColumn(); imgui_print_hex(header.iRelocInfo); ImGui::NextColumn();
	ImGui::Selectable("iTraceMask"); ImGui::NextColumn(); imgui_print_hex(header.iTraceMask); ImGui::NextColumn();
	ImGui::Selectable("iUserDataAddress"); ImGui::NextColumn(); imgui_print_hex(header.iUserDataAddress); ImGui::NextColumn();
	ImGui::Selectable("iTotalUserDataSize"); ImGui::NextColumn(); imgui_print_hex(header.iTotalUserDataSize); ImGui::NextColumn();
}

