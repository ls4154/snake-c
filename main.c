#include <ncurses.h>
#include <time.h>
#include "snake.h"

#define BOARD_WIDTH 16
#define BOARD_HEIGHT 16
#define TICKS_PER_MOVE 20

#define COLOR_SNAKE (' ' | A_REVERSE | COLOR_PAIR(1))
#define COLOR_FOOD (' ' | A_REVERSE | COLOR_PAIR(2))

static int width, height;
static WINDOW *win_game, *win_score;

static struct game g;

static int best_score = 0;

static struct timespec ts;

int main()
{
	/* game board dimensions */
	g.board_width = BOARD_WIDTH;
	g.board_height = BOARD_HEIGHT;

	/* ncurses init */
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	timeout(1);
	curs_set(0);

	ts.tv_sec = 0;
	ts.tv_nsec = 9000000;

	/* set colors */
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLACK); /* snake */
	init_pair(2, COLOR_RED, COLOR_BLACK); /* food */
	init_pair(3, COLOR_CYAN, COLOR_BLUE); /* title */

	refresh();

	/* set window size */
	win_game = newwin(BOARD_HEIGHT + 2, BOARD_WIDTH * 2 + 2, 0, 0);
	win_score = newwin(BOARD_HEIGHT + 2, 12 + 2, 0, BOARD_WIDTH * 2 + 2);

	while (1) {
		init_game(&g);

		wclear(win_game);
		wclear(win_score);

		box(win_game, 0, 0);
		wrefresh(win_game);

		box(win_score, 0, 0);
		wattron(win_score, COLOR_PAIR(3)|A_BOLD|A_BLINK);
		mvwprintw(win_score, 1, 2, "SNAKE GAME");
		wattroff(win_score, COLOR_PAIR(3)|A_BOLD|A_BLINK);
		wattron(win_score, A_BOLD);
		mvwprintw(win_score, 4, 3, "BEST");
		wattroff(win_score, A_BOLD);
		mvwprintw(win_score, 5, 4, "%d", best_score);
		wattron(win_score, A_BOLD);
		mvwprintw(win_score, 7, 3, "SCORE");
		wattroff(win_score, A_BOLD);
		mvwprintw(win_score, 8, 4, "0");
		wrefresh(win_score);

		mvwaddch(win_game, 1+g.snake_tail->row, 1+g.snake_tail->col*2, COLOR_SNAKE);
		waddch(win_game, COLOR_SNAKE);
		mvwaddch(win_game, 1+g.snake_head->row, 1+g.snake_head->col*2, COLOR_SNAKE);
		waddch(win_game,  COLOR_SNAKE);

		mvwaddch(win_game, 1+g.food_row, 1+g.food_col*2, COLOR_FOOD);
		waddch(win_game, COLOR_FOOD);

		wrefresh(win_game);

		int key_input = 4;
		int tick_cnt = 1;
		while (1) {
			switch (getch()) {
			case KEY_UP:
			case 'k':
				key_input = 0;
				break;
			case KEY_RIGHT:
			case 'l':
				key_input = 1;
				break;
			case KEY_DOWN:
			case 'j':
				key_input = 2;
				break;
			case KEY_LEFT:
			case 'h':
				key_input = 3;
				break;
			case 's':
				key_input = 5;
				break;
			case 'q':
				goto A;
				break;
			}

			if (tick_cnt < 0) {
				if (key_input == 5) {
					key_input = 4;
					break;
				}
				nanosleep(&ts, NULL);
				continue;
			}

			if (tick_cnt == TICKS_PER_MOVE) {
				int buf_trow = g.snake_tail->row;
				int buf_tcol = g.snake_tail->col;
				int buf_slen = g.snake_len;
				int buf_frow = g.food_row;
				int buf_fcol = g.food_col;

				if (key_input == 5)
					key_input = 4;

				if (!next_move(&g, key_input)) {
					tick_cnt = -1;
					end_game(&g);

					wattron(win_score, A_BOLD);
					mvwprintw(win_score, 10, 2, "GAME OVER");
					wattroff(win_score, A_BOLD);
					mvwprintw(win_score, 11, 2, "PRESS S");
					mvwprintw(win_score, 12, 2, "TO RESTART");
					wrefresh(win_score);
					continue;
				}


				if (g.snake_len == buf_slen) {
					mvwaddch(win_game, 1+buf_trow, 1+buf_tcol*2, ' ');
					waddch(win_game, ' ');
				} else {
					mvwaddch(win_game, 1+buf_frow, 1+buf_fcol*2, ' ');
					waddch(win_game, ' ');
					mvwaddch(win_game, 1+g.food_row, 1+g.food_col*2, COLOR_FOOD);
					waddch(win_game, COLOR_FOOD);
				}
				mvwaddch(win_game, 1+g.snake_head->row, 1+g.snake_head->col*2, COLOR_SNAKE);
				waddch(win_game, COLOR_SNAKE);

				key_input = 4;
				tick_cnt = 0;

				int score = g.snake_len-2;
				mvwprintw(win_score, 8, 4, "%d", score);
				if (score > best_score)
					best_score = score;
				mvwprintw(win_score, 5, 4, "%d", best_score);

				wrefresh(win_game);
				wrefresh(win_score);
			}
			nanosleep(&ts, NULL);
			++tick_cnt;
		}
	}

A:
	endwin();
	return 0;
}
