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

static void get_grid_pos_from_arr_index(int i, int *x, int *y)
{
        switch(i) {
        case 0:
                *x = 0;
                *y = 0;
                break;
        case 1:
                *x = 0;
                *y = 1;
                break;
        case 2:
                *x = 0;
                *y = 2;
                break;
        case 3:
                *x = 1;
                *y = 0;
                break;
        case 4:
                *x = 1;
                *y = 1;
                break;
        case 5:
                *x = 1;
                *y = 2;
                break;
        case 6:
                *x = 2;
                *y = 0;
                break;
        case 7:
                *x = 2;
                *y = 1;
                break;
        case 8:
                *x = 2;
                *y = 2;
                break;
        default:
                break;
        }

        return;
}

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

static GameState check_row1(ttt *t, PlayerType type)
{
        if (t->arr[0] == type &&
            t->arr[1] == type &&
            t->arr[2] == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_row2(ttt *t, PlayerType type)
{
        if (t->arr[3] == type &&
            t->arr[4] == type &&
            t->arr[5] == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_row3(ttt *t, PlayerType type)
{
        if (t->arr[6] == type &&
            t->arr[7] == type &&
            t->arr[8] == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_col1(ttt *t, PlayerType type)
{
        if (t->arr[0] == type &&
            t->arr[3] == type &&
            t->arr[6] == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_col2(ttt *t, PlayerType type)
{
        if (t->arr[1] == type &&
            t->arr[4] == type &&
            t->arr[7] == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_col3(ttt *t, PlayerType type)
{
        if (t->arr[2] == type &&
            t->arr[5] == type &&
            t->arr[8] == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_dia1(ttt *t, PlayerType type)
{
        if (t->arr[0] == type &&
            t->arr[4] == type &&
            t->arr[8] == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_dia2(ttt *t, PlayerType type)
{
        if (t->arr[2] == type &&
            t->arr[4] == type &&
            t->arr[6] == type)
		return GAME_STATE_COMPLETE;
	else
		return GAME_STATE_INCOMPLETE;
}

static GameState check_win(ttt *t, PlayerType type)
{
	GameState state;

	if (check_row1(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_row2(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_row3(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_col1(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_col2(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_col3(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_dia1(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else if (check_dia2(t, type) == GAME_STATE_COMPLETE) {
		state = GAME_STATE_COMPLETE;
	} else {
		state = GAME_STATE_INCOMPLETE;
	}

	return state;
}

static void game_array_init(ttt *t)
{
        int i;

        for (i = 0; i < 9; i++)
                t->arr[i] = 0;

        t->freecells = 9;
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

static PlayerType who_wins(ttt *t)
{
        PlayerType type = PLAYER_TYPE_NONE;
        GameState state;

        state = check_win(t, PLAYER_TYPE_COMPUTER);
        if (state == GAME_STATE_COMPLETE)
                type = PLAYER_TYPE_COMPUTER;
        else {
                state = check_win(t, PLAYER_TYPE_PLAYER1);
                if (state == GAME_STATE_COMPLETE)
                        type = PLAYER_TYPE_PLAYER1;
        }

        return type;
}

static int compute_moves(ttt *t, PlayerType ptype)
{
        int i;
        int weight = -2;
        int xpos = -1, ypos = -1;
        PlayerType player;

        player = who_wins(t);
        if (player != PLAYER_TYPE_NONE) {
                return player * ptype;
        }

        for (i = 0; i < 9; i++) {
                if (t->arr[i] == PLAYER_TYPE_NONE) {
                        int temp_weight;
                        t->arr[i] = ptype;
                        temp_weight = SWITCH_PLAYER * compute_moves(t, ptype * SWITCH_PLAYER); /* Switch player */
                        if (temp_weight > weight) {
                                weight = temp_weight;
                                get_grid_pos_from_arr_index(i, &xpos, &ypos);
                        }
                        t->arr[i] = PLAYER_TYPE_NONE;
                }
        }

        if (xpos == -1 || ypos == -1)
                return 0;

        return weight;
}

static void computer_make_move(ttt *t)
{
	Game *game = t->game;
        ElementType type = game->cur_player->element;
        int i;
        int xpos = -1, ypos = -1;
        int weight = -2;

        for (i = 0; i < 9; i++) {
                if (t->arr[i] == PLAYER_TYPE_NONE) {
                        int temp_weight;
                        t->arr[i] = PLAYER_TYPE_COMPUTER;
                        temp_weight = SWITCH_PLAYER * compute_moves(t, PLAYER_TYPE_PLAYER1);
                        t->arr[i] = PLAYER_TYPE_NONE;
                        if (temp_weight > weight) {
                                weight = temp_weight;
                                get_grid_pos_from_arr_index(i, &xpos, &ypos);
                        }
                }
        }

        if (xpos != -1 && ypos != -1) {
                t->grid[xpos][ypos].type = type;
                t->arr[3 * xpos+ ypos] = type;

                if (type == ELEMENT_TYPE_O) {
                        t->grid[xpos][ypos].type = ELEMENT_TYPE_O;
                        draw_o(&t->grid[xpos][ypos]);
                } else if (type == ELEMENT_TYPE_X) {
                        t->grid[xpos][ypos].type = ELEMENT_TYPE_X;
                        draw_x(&t->grid[xpos][ypos]);
                }
                set_cur_pos(t, xpos, ypos);
                redraw_grid(t, xpos, ypos);
        }

        t->freecells--;

	player_boxes_refresh_title(t);

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
	GameState game_state = GAME_STATE_INCOMPLETE;

        game_array_init(t);

	nodelay(stdscr, FALSE);

	/* First move */
	if ((t->game->game_type == SINGLE_PLAYER) &&
	    (t->game->cur_player->player == PLAYER_TYPE_COMPUTER)) {
		computer_make_move(t);
		player_boxes_refresh_title(t);
		refresh();
	}

	do {
		ElementType type;
		type = ELEMENT_TYPE_NONE;

		if (t->game->cur_player->player == PLAYER_TYPE_COMPUTER) {
                        type = t->game->cur_player->element;
			computer_make_move(t);
		} else {
			ch = getch();

			if (ch == KEY_ESC || ch == 'q' || ch == 'Q') {
                                printf("quitting mate!!!\n");
				break;
                        }

			if (game_state == GAME_STATE_COMPLETE ||
                            game_state == GAME_STATE_DRAW) {
				if (ch != 'N' || ch != 'n')
					continue;
				else
					printf("display new game menu here.\n");
			}

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
#if 0 /* We won't use these keys for now. Maybe later?*/
			case 'o':
			case 'O':
				if (t->grid[cur_x_pos][cur_y_pos].type == ELEMENT_TYPE_NONE) {
					type = ELEMENT_TYPE_O;
					t->grid[cur_x_pos][cur_y_pos].type = type;
					draw_o(&t->grid[cur_x_pos][cur_y_pos]);
                                        t->arr[3*cur_x_pos+cur_y_pos] = PLAYER_TYPE_PLAYER1;
					wnoutrefresh(t->board);
				}
				draw_player_boxes(t, 0);
				break;
			case 'x':
			case 'X':
				if (t->grid[cur_x_pos][cur_y_pos].type == ELEMENT_TYPE_NONE) {
					type = ELEMENT_TYPE_X;
					t->grid[cur_x_pos][cur_y_pos].type = type;
					draw_x(&t->grid[cur_x_pos][cur_y_pos]);
                                        t->arr[3*cur_x_pos+cur_y_pos] = PLAYER_TYPE_PLAYER1;
					wnoutrefresh(t->board);
				}
				draw_player_boxes(t, 1);
				break;
#endif
			case '\n':
				type = t->game->cur_player->element;
				t->grid[cur_x_pos][cur_y_pos].type = type;
                                t->arr[3*cur_x_pos+cur_y_pos] = PLAYER_TYPE_PLAYER1;
				if (type == ELEMENT_TYPE_X)
					draw_x(&t->grid[cur_x_pos][cur_y_pos]);
				else
					draw_o(&t->grid[cur_x_pos][cur_y_pos]);

				_update_current_player(t);
                                t->freecells--;
				break;
			default:
				continue;
			}
		}

		refresh();
		player_boxes_refresh_title(t);
		redraw_grid(t, cur_x_pos, cur_y_pos);
		set_cur_pos(t, cur_x_pos, cur_y_pos);

                if (check_win(t, PLAYER_TYPE_COMPUTER) == GAME_STATE_COMPLETE) {
                        mvwaddstr(t->msg, 3, 25, "Computer Wins!!");
                        game_state = GAME_STATE_COMPLETE;
                } else if (check_win(t, PLAYER_TYPE_PLAYER1) == GAME_STATE_COMPLETE) {
                        mvwaddstr(t->msg, 3, 25, "You Win!!");
                        game_state = GAME_STATE_COMPLETE;
                }
                wnoutrefresh(t->msg);

                if (t->freecells <= 0 &&
                    game_state == GAME_STATE_INCOMPLETE) {
                        game_state = GAME_STATE_DRAW;
                        mvwaddstr(t->msg, 3, 25, "It's a Draw!!");
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
