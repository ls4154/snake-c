#include <stdlib.h>
#include <time.h>
#include "snake.h"

static const int dr[4] = {-1, 0, 1, 0}, dc[4] = {0, 1, 0, -1};

void init_game(struct game *g)
{
	srand(time(NULL));

	g->snake_len = 2;
	g->snake_dir = 1;

	g->food_col = -1;
	g->food_row = -1;

	g->snake_tail = NEW_POINT;
	g->snake_tail->row = g->board_width / 2 - 1;
	g->snake_tail->col = g->board_height / 2 - 1;

	g->snake_head = NEW_POINT;
	g->snake_head->row = g->board_width / 2 - 1;
	g->snake_head->col = g->board_height / 2;
	g->snake_head->next = NULL;

	g->snake_tail->next = g->snake_head;

	add_food(g);
}

void add_food(struct game *g)
{
	int row, col;

	while (1) {
		row = rand() % g->board_height;
		col = rand() % g->board_width;
		for(struct point_list *ptr = g->snake_tail; ptr; ptr = ptr->next)
			if(ptr->row == row && ptr->col == col)
				goto B;
		if(g->food_row == row && g->food_col == col)
			goto B;

		break;
B:
		continue;
	}
	g->food_row = row;
	g->food_col = col;
}

void end_game(struct game *g)
{
	for (struct point_list *ptr = g->snake_tail, *next_ptr; ptr; ptr = next_ptr) {
		next_ptr = ptr->next;
		free(ptr);
	}
	g->snake_head = NULL;
	g->snake_tail = NULL;
	g->snake_len = 0;
	g->food_col = -1;
	g->food_row = -1;
}

int next_move(struct game *g, int input_dir)
{
	int row, col, ndir;
	struct point_list *buf;

	int dif = (input_dir - g->snake_dir + 4) % 4;
	if (input_dir != 4 && (dif == 1 || dif == 3))
		g->snake_dir = input_dir;

	row = g->snake_head->row + dr[g->snake_dir];
	col = g->snake_head->col + dc[g->snake_dir];

	if (row < 0 || col < 0 || row >= g->board_height || col >= g->board_width)
		goto D;

	for (struct point_list *ptr = g->snake_tail; ptr != g->snake_head; ptr = ptr->next)
		if (row == ptr->row && col == ptr->col)
			goto D;

	if (row == g->food_row && col == g->food_col) {
		++g->snake_len;
		add_food(g);
		goto C;
	}

	buf = g->snake_tail;
	g->snake_tail = g->snake_tail->next;
	free(buf);

C:
	buf = NEW_POINT;
	buf->row = row;
	buf->col = col;
	buf->next = NULL;

	g->snake_head->next = buf;
	g->snake_head = buf;

	return 1;
D:
	return 0;
}

