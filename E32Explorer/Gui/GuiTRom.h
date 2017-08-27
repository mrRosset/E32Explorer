#pragma once

#include "Gui.h"
#include "../TRomImage.h"
#include "MemoryEditor.h"


class GuiTRomImage : public Gui
{
private:
	bool show_header_window = true;
	bool show_metadata_window = true;
	bool show_export_dir_window = true;
	void render_header_window(TRomImageHeader& header);
	void render_flags_window(TRomImageHeader& header);
	void render_export_windows();
public:
	TRomImage& image;
	GuiTRomImage(TRomImage& image, std::string additional_title);
	bool render();

};
