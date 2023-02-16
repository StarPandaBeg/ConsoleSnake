#include <iostream>
#include <conio.h>

#include "Game.h"

int keymap[] = { 'w', 'd', 's', 'a' };
size_t keymapSize = sizeof(keymap) / sizeof(int);
int* keymapEnd = keymap + keymapSize;

std::string make_duplicate(const std::string& str, unsigned int x) {
    std::string newstr;
    if (x > 0) {
        unsigned int y = 2;
        newstr.reserve(str.length() * x);
        newstr.append(str);
        while (y < x) {
            newstr.append(newstr);
            y *= 2;
        }
        newstr.append(newstr.c_str(), (x - y / 2) * str.length());
    }
    return newstr;
}

void draw_walls(struct Desk* desk) {
    std::string boardWall("");
    boardWall.insert(0, desk->width + 2, 'X');
    boardWall.insert(desk->width + 2, "\n");

    std::string boardLine("XX\n");
    boardLine.insert(1, desk->width, ' ');

    std::string board("");
    board.insert(0, boardWall);
    board.insert(desk->width + 3, make_duplicate(boardLine, desk->height));
    board.insert((desk->height + 1) * (desk->width + 3), boardWall);

    std::cout << board;
}

void draw_score_board(struct Desk* desk) {
    std::string scoreWall("");
    scoreWall.insert(0, 17, 'X');
    scoreWall.insert(17, "\n");

    gotoxy(desk->width + 2, 1);
    std::cout << scoreWall;
    gotoxy(desk->width + 2, desk->height + 2);
    std::cout << scoreWall;

    for (int i = 1; i <= desk->height + 2; i++) {
        gotoxy(desk->width + 19, i);
        std::cout << "X";
    }

    gotoxy(desk->width + 7, 2);
    std::cout << "Snake v1";
}

void draw_score(struct Desk* desk) {
    gotoxy(desk->width + 4, 4);
    std::cout << "Score: " << (desk->length - SNAKE_START_LENGTH) * 100;
}

struct Desk* create_desk(int width, int height) {
    struct Desk* d = new struct Desk;
    struct Point* head = new struct Point;

    d->width = width;
    d->height = height;
    d->desk = new int* [height];
    for (int h = 0; h < height; h++)
    {
        d->desk[h] = new int[width];
        for (int w = 0; w < width; w++)
        {
            d->desk[h][w] = 0;
        }
    }

    head->x = 5;
    head->y = 5;
    d->head = head;
    d->desk[5][5] = d->length;

	return d;
}

struct Point* generate_apple(struct Desk* desk) {
    int x, y;
    do {
        x = rand() % desk->width;
        y = rand() % desk->height;
    } while (desk->desk[y][x] != 0);
    return new Point{ x, y };
}

void init(struct Desk* desk) {
    home();
    draw_walls(desk);
    draw_score_board(desk);
}

int handle() {
    if (_kbhit()) {
        int c = _getch();
        int* result = std::find(keymap, keymapEnd, c);
        
        if (result != keymapEnd) {
            int index = std::distance(keymap, result);
            return index;
        }
    }
    return -1;
}

int validate(int olddir, int newdir) {
    if (newdir == DIR_NONE) return olddir;
    if (abs(newdir - olddir) == 2) return olddir;
    return newdir;
}

void update(struct Desk* desk, int dir, bool& game_over) {
    switch (dir) {
        case DIR_UP:
            desk->head->y = desk->head->y <= 0 ? desk->height - 1 : desk->head->y - 1;
            break;
        case DIR_DOWN:
            desk->head->y = desk->head->y >= desk->height - 1 ? 0 : desk->head->y + 1;
            break;
        case DIR_RIGHT:
            desk->head->x = desk->head->x >= desk->width - 1 ? 0 : desk->head->x + 1;
            break;
        case DIR_LEFT:
            desk->head->x = desk->head->x <= 0 ? desk->width - 1 : desk->head->x - 1;
            break;
    }
    if (desk->apple != NULL && desk->apple->x == desk->head->x && desk->head->y == desk->apple->y) {
        desk->length++;
        desk->apple = NULL;
    }
    else if (desk->desk[desk->head->y][desk->head->x] != 0) {
        game_over = true;
        return;
    }
    else {
        for (int h = 0; h < desk->height; h++) {
            for (int w = 0; w < desk->width; w++) {
                if (desk->desk[h][w] > 0) desk->desk[h][w]--;
            }
        }
    }

    desk->desk[desk->head->y][desk->head->x] = desk->length;
}

void draw(struct Desk* desk) {
    bhome();
    draw_score(desk);
    for (int h = 0; h < desk->height; h++) {
        for (int w = 0; w < desk->width; w++) {
            int val = desk->desk[h][w];
            char c = ' '; //+ 48;
            if (desk->apple != NULL && h == desk->apple->y && w == desk->apple->x) c = 'A';

            if (val > 0) c = 'O';
            if (val == 1) c = 'o';
            if (val == desk->length) c = '0';

            gotoxy(2 + w, 2 + h);
            std::cout << c;
        }
    }
}