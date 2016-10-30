#pragma once

#include "Gui.h"
#include "..\E32Image.h"

class GuiE32Image: public Gui
{
public:
	E32Image& image;
	GuiE32Image(E32Image& image, std::string additional_title);
	bool render();
};

