#pragma once

#include "Gui.h"
#include "../TRomImage.h"
#include "MemoryEditor.h"


class GuiTRomImage : public Gui
{
private:
	bool show_header_window = true;
	bool show_metadata_window = true;
	void render_header_window(TRomImageHeader& header);
	void render_flags_window(TRomImageHeader & header);
public:
	TRomImage& image;
	GuiTRomImage(TRomImage& image, std::string additional_title);
	bool render();

};
