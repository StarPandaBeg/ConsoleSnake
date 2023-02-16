#include <iostream>
#include <chrono>
#include <thread>

#define WIDTH 100
#define HEIGHT 25

#include "Game.h"
#include "ConsoleNavigation.h"

int main() {
	struct Desk* desk = create_desk(WIDTH, HEIGHT);
	hide_cursor();

	bool gameover = false;
	int dir = DIR_DOWN;

	init(desk);
	while (!gameover) {
		if (desk->apple == NULL) desk->apple = generate_apple(desk);

		dir = validate(dir, handle());
		update(desk, dir, gameover);
		draw(desk);

		std::this_thread::sleep_for(std::chrono::milliseconds(60));
	}
	clrscr();

	return 0;
}