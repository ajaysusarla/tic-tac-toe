/*
 * tic-tac-toe
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "game.h"
#include "draw.h"

#include <stdlib.h>

static void get_cur_pos(ttt *t, int *cur_x_pos, int *cur_y_pos)
{
	if (t != NULL) {
		*cur_x_pos = t->cur_x;
		*cur_y_pos = t->cur_y;
	}
}

static void set_cur_pos(ttt *t, int cur_x_pos, int cur_y_pos)
{
	t->cur_x = cur_x_pos;
	t->cur_y = cur_y_pos;
}

static void do_key_up(ttt *t, int *cur_x_pos, int *cur_y_pos)
{
	if ((*cur_x_pos - 1) >= 0)
		*cur_x_pos = *cur_x_pos - 1;
	else
		*cur_x_pos = 2 - *cur_x_pos;
}

static void do_key_down(ttt *t, int *cur_x_pos, int *cur_y_pos)
{
	if ((*cur_x_pos + 1) > 2)
		*cur_x_pos = 0;
	else
		*cur_x_pos = *cur_x_pos + 1;
}

static void do_key_left(ttt *t, int *cur_x_pos, int *cur_y_pos)
{
	if ((*cur_y_pos - 1) >= 0)
		*cur_y_pos = *cur_y_pos - 1;
	else
		*cur_y_pos = 2 - *cur_y_pos;
}

static void do_key_right(ttt *t, int *cur_x_pos, int *cur_y_pos)
{
	if ((*cur_y_pos + 1) > 2)
		*cur_y_pos = 0;
	else
		*cur_y_pos = *cur_y_pos + 1;
}

static GameState check_row_1(ttt *t, ElementType type)
{
	if (t->grid[0][0].type == type &&
	    t->grid[0][1].type == type &&
	    t->grid[0][2].type == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_row_2(ttt *t, ElementType type)
{
	if (t->grid[1][0].type == type &&
	    t->grid[1][1].type == type &&
	    t->grid[1][2].type == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_row_3(ttt *t, ElementType type)
{
	if (t->grid[2][0].type == type &&
	    t->grid[2][1].type == type &&
	    t->grid[2][2].type == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_col_1(ttt *t, ElementType type)
{
	if (t->grid[0][0].type == type &&
	    t->grid[1][0].type == type &&
	    t->grid[2][0].type == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_col_2(ttt *t, ElementType type)
{
	if (t->grid[0][1].type == type &&
	    t->grid[1][1].type == type &&
	    t->grid[2][1].type == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_col_3(ttt *t, ElementType type)
{
	if (t->grid[0][2].type == type &&
	    t->grid[1][2].type == type &&
	    t->grid[2][2].type == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_dia_1(ttt *t, ElementType type)
{
	if (t->grid[0][0].type == type &&
	    t->grid[1][1].type == type &&
	    t->grid[2][2].type == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_dia_2(ttt *t, ElementType type)
{
	if (t->grid[0][2].type == type &&
	    t->grid[1][1].type == type &&
	    t->grid[2][0].type == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_for_win(ttt *t, ElementType type)
{
	GameState state;

	if (check_row_1(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_row_2(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_row_3(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_col_1(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_col_2(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_col_3(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_dia_1(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_dia_2(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else {
		state = GAME_STATE_INCOMPLETE;
	}

	return state;
}

static void _update_current_player(ttt *t)
{
	Game *game = t->game;

	if (game->cur_player == &game->player[0]) {
		game->cur_player = &game->player[1];
		draw_player_boxes(t, 1);
	} else if (game->cur_player == &game->player[1]) {
		game->cur_player = &game->player[0];
		draw_player_boxes(t, 0);
	}
}

static void _computer_make_move(ttt *t, int x, int y)
{
	Game *game = t->game;

	if (game->cur_player->element == ELEMENT_TYPE_O) {
		t->grid[x][y].type = ELEMENT_TYPE_O;
		draw_o(&t->grid[x][y]);
	} else if (game->cur_player->element == ELEMENT_TYPE_X) {
		t->grid[x][y].type = ELEMENT_TYPE_X;
		draw_x(&t->grid[x][y]);
	}

	player_boxes_refresh_title(t);
	set_cur_pos(t, x, y);
	redraw_grid(t, x, y);

	wnoutrefresh(t->board);
	_update_current_player(t);
}

void player_boxes_refresh_title(ttt *t)
{
	wattrset(t->p[0], A_BOLD);
	mvwaddstr(t->p[0], 1, 8, t->game->player[0].player_str);
	wnoutrefresh(t->p[0]);
	wattrset(t->p[1], A_BOLD);
	mvwaddstr(t->p[1], 1, 8, t->game->player[1].player_str);
	wnoutrefresh(t->p[1]);
}


void game_loop(ttt *t)
{
	int ch = 0;
	int cur_x_pos = 0;
	int cur_y_pos = 0;

	nodelay(stdscr, FALSE);

	/* First move */
	if ((t->game->game_type == SINGLE_PLAYER) &&
	    (t->game->cur_player->player == PLAYER_TYPE_COMPUTER)) {
		_computer_make_move(t, cur_x_pos, cur_y_pos);
		player_boxes_refresh_title(t);
		refresh();
	}

	do {
		ElementType type;
		type = ELEMENT_TYPE_NONE;

		if (t->game->cur_player->player == PLAYER_TYPE_COMPUTER) {
			cur_x_pos = 1;
			cur_y_pos = 1;
			_computer_make_move(t, cur_x_pos, cur_y_pos);
		} else {
			ch = getch();


			if (ch == KEY_ESC || ch == 'q' || ch == 'Q')
				break;

			switch(ch) {
			case KEY_UP:
			case 'W':
			case 'w':
				do_key_up(t, &cur_x_pos, &cur_y_pos);
				break;
			case KEY_DOWN:
			case 'S':
			case 's':
				do_key_down(t, &cur_x_pos, &cur_y_pos);
				break;
			case KEY_LEFT:
			case 'A':
			case 'a':
				do_key_left(t, &cur_x_pos, &cur_y_pos);
				break;
			case KEY_RIGHT:
			case 'D':
			case 'd':
				do_key_right(t, &cur_x_pos, &cur_y_pos);
				break;
			case 'o':
			case 'O':
				if (t->grid[cur_x_pos][cur_y_pos].type == ELEMENT_TYPE_NONE) {
					type = ELEMENT_TYPE_O;
					t->grid[cur_x_pos][cur_y_pos].type = type;
					draw_o(&t->grid[cur_x_pos][cur_y_pos]);
					wnoutrefresh(t->board);
				}
//				draw_player_boxes(t, 0);
				break;
			case 'x':
			case 'X':
				if (t->grid[cur_x_pos][cur_y_pos].type == ELEMENT_TYPE_NONE) {
					type = ELEMENT_TYPE_X;
					t->grid[cur_x_pos][cur_y_pos].type = type;
					draw_x(&t->grid[cur_x_pos][cur_y_pos]);
					wnoutrefresh(t->board);
				}
//				draw_player_boxes(t, 1);
				break;
			case '\n':
				type = t->game->cur_player->element;
				t->grid[cur_x_pos][cur_y_pos].type = type;
				if (type == ELEMENT_TYPE_X)
					draw_x(&t->grid[cur_x_pos][cur_y_pos]);
				else
					draw_o(&t->grid[cur_x_pos][cur_y_pos]);

				_update_current_player(t);
				break;
			default:
				continue;
			}
		}

		refresh();
		player_boxes_refresh_title(t);
		redraw_grid(t, cur_x_pos, cur_y_pos);
		set_cur_pos(t, cur_x_pos, cur_y_pos);
		if ((type != ELEMENT_TYPE_NONE) &&
		    (check_for_win(t, type) == GAME_STATE_COMPLETE)) {
			mvwaddstr(t->msg, 3, 25, "Game WON!!!");
			wnoutrefresh(t->msg);
		}

		refresh();

	} while (1);

	nodelay(stdscr, FALSE);
}

Game * game_new(void)
{
	Game *game;

	game = (Game *)malloc(sizeof(Game));
	if (game == NULL) {
		fprintf(stderr, "Memory allocation failure\n");
		return NULL;
	}

	return game;
}

void game_free(Game *game)
{
	if (game != NULL) {
		free(game);
		game = NULL;
	}
}

void game_init_single_player(Game *game)
{
	game->game_type = SINGLE_PLAYER;
	game->game_state = GAME_STATE_INCOMPLETE;

	game->player[0].player = PLAYER_TYPE_PLAYER1;
	game->player[0].element = ELEMENT_TYPE_X;
	game->player[0].player_str = YOU_STRING;

	game->player[1].player = PLAYER_TYPE_COMPUTER;
	game->player[1].element = ELEMENT_TYPE_O;
	game->player[1].player_str = COMPUTER_STRING;

	game->cur_player = &game->player[0];
}

void game_init_single_player_computer_first(Game *game)
{
	game->game_type = SINGLE_PLAYER;
	game->game_state = GAME_STATE_INCOMPLETE;

	game->player[0].player = PLAYER_TYPE_COMPUTER;
	game->player[0].element = ELEMENT_TYPE_X;
	game->player[0].player_str = COMPUTER_STRING;

	game->player[1].player = PLAYER_TYPE_PLAYER1;
	game->player[1].element = ELEMENT_TYPE_O;
	game->player[1].player_str = YOU_STRING;

	game->cur_player = &game->player[0];

}

void game_init_multi_player(Game *game)
{
}
