#pragma once

#include "Gui.h"
#include "..\TRomImage.h"
#include "MemoryEditor.h"


class GuiTRomImage : public Gui
{
private:
	bool show_header_window = true;
	void render_header_window(TRomImageHeader& header);
public:
	TRomImage& image;
	GuiTRomImage(TRomImage& image, std::string additional_title);
	bool render();

};