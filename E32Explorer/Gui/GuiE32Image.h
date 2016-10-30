#pragma once

#include "Gui.h"
#include "..\E32Image.h"
#include "MemoryEditor.h"


class GuiE32Image: public Gui
{
private:
	bool show_test_window = true;
	bool show_header_window = true;
	bool show_sections_window = true;
	bool show_metadata_window = true;
	bool show_iat_window = true;
	bool show_export_dir_window = true;
	bool show_import_section = true;
	MemoryEditor memory_editor;
public:
	E32Image& image;
	GuiE32Image(E32Image& image, std::string additional_title);
	bool render();
};

