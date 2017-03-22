#include <iostream>
#include <chrono>
#include <thread>
#include "E32Image.h"
#include "Loader.h"
#include "Gui/GuiE32Image.h"
#include "Gui/GuiTRomImage.h"

std::string extract_filename(const std::string& filepath)
{
	auto pos = filepath.rfind("\\");
	if (pos == std::string::npos)
		pos = -1;
	return std::string(filepath.begin() + pos + 1, filepath.end());
}

int main(int argc, char *argv[])
{
	if (argc > 1) {
		//First try with E32Image
		E32Image file;
		bool worked = loader::load(argv[1], file);

		Gui* gui = nullptr;

		if (worked) {
			gui = new GuiE32Image(file, extract_filename(std::string(argv[1])));
		}
		else {
			//next try: TRomImage
			TRomImage file;
			bool worked = loader::load(argv[1], file);

			if (!worked) {
				//not a know format
				return -1;
			}

			gui = new GuiTRomImage(file, extract_filename(std::string(argv[1])));
		}

		bool running = true;

		const int FRAMES_PER_SECOND = 25;
		const uint64_t SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
		auto clock = std::chrono::high_resolution_clock::now().time_since_epoch();
		auto next_game_tick = std::chrono::duration_cast<std::chrono::milliseconds>(clock).count();
		uint64_t sleep_time = 0;

		while (running) {
			running =gui->render();

			next_game_tick += SKIP_TICKS;

			clock = std::chrono::high_resolution_clock::now().time_since_epoch();
			auto tickCount = std::chrono::duration_cast<std::chrono::milliseconds>(clock).count();
			sleep_time = next_game_tick - tickCount;

			if (sleep_time > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
			}
		}

		delete gui;
	}

	return 0;

}