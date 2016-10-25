#include <iostream>
#include "E32Image.h"
#include "Loader.h"
#include "Gui.h"

int main(int argc, char *argv[])
{
	if (argc > 1) {
		E32Image file;
		bool err = loader::load(argv[1], file);

		if (!err) {
			return -1;
		}

		Gui gui;
		bool running = true;

		const int FRAMES_PER_SECOND = 25;
		const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
		DWORD next_game_tick = GetTickCount();
		int sleep_time = 0;

		while (running) {
			running =gui.render(file);

			next_game_tick += SKIP_TICKS;
			sleep_time = next_game_tick - GetTickCount();
			if (sleep_time >= 0) {
				Sleep(sleep_time);
			}
			else {
				// Shit, we are running behind!
			}
		}
	}

	return 0;

}