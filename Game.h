#pragma once

#include "ConsoleNavigation.h"

#define DIR_NONE -1
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define SNAKE_START_LENGTH 5

struct Point {
	int x;
	int y;
};

struct Desk {
	int** desk;
	int width;
	int height;

	struct Point* head = NULL;
	int length = SNAKE_START_LENGTH;
	struct Point* apple = NULL;
};

struct Desk* create_desk(int width, int height);
void init(struct Desk* desk);
int handle();
int validate(int olddir, int newdir);
struct Point* generate_apple(struct Desk* desk);
void update(struct Desk* desk, int dir, bool& game_over);
void draw(struct Desk* desk);