#ifndef SNAKE_H
#define SNAKE_H

#define NEW_POINT (Point_list *)malloc(sizeof(Point_list))

typedef struct point_list {
	int row, col;
	struct point_list *next;
} Point_list;

typedef struct game {
	int board_width, board_height;
	Point_list *snake_tail, *snake_head;
	int snake_len;
	int snake_dir;
	int food_row, food_col;
} Game;

void init_game(Game *g);

void end_game(Game *g);

void add_food(Game *g);

int next_move(Game *g, int input_dir);

#endif
