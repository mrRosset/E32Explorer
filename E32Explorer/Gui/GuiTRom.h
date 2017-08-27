#pragma once

#include "Gui.h"
#include "../TRom.h"

class GuiTRom : public Gui
{
private:
	bool show_header_window = true;
	void render_header_window();
public:
	TRom& image;
	GuiTRom(TRom& image, std::string additional_title);
	bool render();

};
