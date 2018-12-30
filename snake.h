#ifndef SNAKE_H
#define SNAKE_H

#define NEW_POINT malloc(sizeof(struct point_list))

struct point_list {
	int row, col;
	struct point_list *next;
};

struct game {
	int board_width, board_height;
	struct point_list *snake_tail, *snake_head;
	int snake_len;
	int snake_dir;
	int food_row, food_col;
};

void init_game(struct game *g);

void end_game(struct game *g);

void add_food(struct game *g);

int next_move(struct game *g, int input_dir);

#endif
