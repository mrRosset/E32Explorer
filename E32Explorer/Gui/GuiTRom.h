#pragma once

#include "Gui.h"
#include "../TRom.h"

class GuiTRom : public Gui
{
private:
	bool show_header_window = true;
	bool show_dir_list_window = true;
	void render_header_window();
	void render_dir_list_window();
public:
	TRom& image;
	GuiTRom(TRom& image, std::string additional_title);
	bool render();

};
