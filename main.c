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

#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>
#include <libgen.h>
#include <term.h>
#include <sys/ioctl.h>
#include <math.h>

typedef enum {
	ELEMENT_TYPE_X,
	ELEMENT_TYPE_O,
	ELEMENT_TYPE_NONE,
	NUM_ELEMENTS,
} ElementType;

typedef struct {
	WINDOW *element;
	ElementType type;
	int xoff;
	int yoff;
} Grid;

struct _ttt {
	const char *title;
	WINDOW *border;
	WINDOW *board;
	WINDOW *msg;
	Grid grid[3][3];
	int cur_x;
	int cur_y;
};

typedef struct _ttt ttt;

#define DEG_TO_RAD(deg) (deg * (180.0f/M_PI))
#define ELEMENT_HT 9
#define ELEMENT_WD 20

/* Keys */
#define KEY_ESC 27

/* help */
/* Long options. */
static struct option long_options [] = {
	{ "help", no_argument, NULL, 'h' },
	{ "version", no_argument, NULL, 'V' },
	{ NULL, 0, NULL, 0 }
};

static void usage(char *program_name)
{
        printf("%s - %s\n\n", program_name, "A curses based game of tic-tac-toe.");
        printf("Usage: %s [OPTIONS...] \n", program_name);
        printf("   -h, --help,        display this help and exit\n");
        printf("   -V, --version,     output version information and exit\n\n");

        exit(EXIT_FAILURE);
}

static void help(char *program_name)
{
        printf("%s - %s\n\n", program_name, "A curses based game of tic-tac-toe.");
        printf("**  Instructions  **\n\n");
        printf("    KEY         -       ACTION\n");
        printf("----------------------------------\n");
        printf("Left Arrow  (or) A :  Move left.\n");
        printf("Right Arrow (or) D :  Move right.\n");
        printf("Up Arrow    (or) W :  Move up.\n");
        printf("Down Arrow  (or) S :  Move down.\n");
        printf("Enter              :  Select entry.\n");
        printf("p/P                :  Pause the game.\n");
        printf("q/Q/C-c/Esc        :  Quit the game.\n");

        printf("\n");

        printf("**     Options    **\n\n");
        printf("Usage: %s [OPTIONS...] \n", program_name);
        printf("   -h, --help,        display this help and exit\n");
        printf("   -V, --version,     output version information and exit\n\n");

        exit(EXIT_SUCCESS);
}

static void version(char *program_name)
{
        printf("%s %s\n\n", program_name, VERSION);

        printf("Written by %s.\n","Parthasarathi Susarla");

        exit(EXIT_SUCCESS);
}

static int parse_args(char *program_name, int argc, char **argv)
{
        int opt;
        //int optind = 0;
        int do_help;
        int do_version;

        /* Default values */
        do_help = FALSE;
        do_version = FALSE;

        while (1) {
                opt = getopt_long(argc, argv, "hV", long_options, NULL);
                if (opt == -1)
                        break;

                switch (opt) {
                case '\0':
                        break;

                case 'h':
                        do_help = TRUE;
                        break;

                case 'V':
                        do_version = TRUE;
                        break;

                default:
                        usage(program_name);
                        break;
                }
        }

        /* Version information is requested.  */
        if (do_version)
                version(program_name);

        /* Help is requested.  */
        if (do_help)
                help(program_name);

        return 0;
}

/* toggle cursor display */
static void _hide_cursor(int state)
{
	if (state == TRUE)
		curs_set(0);
	else if (state == FALSE)
		curs_set(1);
	else
		printf("interface_hide_cursor: Invalid state\n");
}

static int screen_init(void)
{
	/* ncurses initialisation */
	if (!initscr()) {
		return -1;
	}

	if (getmaxx(stdscr) < 140 && getmaxy(stdscr) < 40) {
		endwin();
		fprintf(stderr, "we need screensize more than 140x40\n");
		return -1;
	}

	keypad(stdscr, TRUE);  /* Enable keypad for the user's terminal */
	nodelay(stdscr, TRUE); /* non-blocking mode*/
	cbreak();              /* Read single input chars, without waiting for \n*/
	noecho();              /* Don't echo input */

	_hide_cursor(TRUE);

	return 0;
}

static void screen_fin(void)
{
	endwin();
}

static void termination_handler(int sig)
{
	screen_fin();
	exit(EXIT_SUCCESS);
}

/*
 * Set window to attribute 'attr'
 */
static void _window_attr_clear(WINDOW * win, int height, int width, chtype attr)
{
        int i, j;

        wattrset(win, attr);

        for (i = 0; i < height; i++) {
                wmove(win, i, 0);
                for (j = 0; j < width; j++)
                        waddch(win, ' ');
        }

        touchwin(win);
}

static void _window_draw_box(WINDOW *win,
			     int y, int x,
			     int height, int width,
			     chtype box, chtype border)
{
	int i, j;

	for (i = 0; i < height; i++) {
		wmove(win, y + i, x);
		for (j = 0; j < width - 2; j++)
			if (!i && !j)
				waddch(win, border | ACS_ULCORNER);
			else if (i == height - 1 && !j)
				waddch(win, border | ACS_LLCORNER);
			else if (!i && j == width - 3)
				waddch(win, box | ACS_URCORNER);
			else if (i == height - 1 && j == width - 3)
				waddch(win, box | ACS_LRCORNER);
			else if (!i)
				waddch(win, border | ACS_HLINE);
			else if (i == height - 1)
				waddch(win, box | ACS_HLINE);
			else if (!j)
				waddch(win, border | ACS_VLINE);
			else if (j == width - 3)
				waddch(win, box | ACS_VLINE);
			else
				waddch(win, box | ' ');
	}
}

static void _window_clear(ttt *t)
{
	_window_attr_clear(stdscr, LINES, COLS, A_NORMAL);

        if (t->title != NULL) {
                int i;

                wattrset(stdscr, A_BOLD);
                mvwaddstr(stdscr, 0, 1, (char *)t->title);
                wmove(stdscr, 1, 1);
                for (i = 1; i < COLS - 1; i++)
                        waddch(stdscr, ACS_HLINE);
        }
        wnoutrefresh(stdscr);
}


static void draw_o(Grid *element)
{
	float deg, w, h;
	int y, x;

	wattrset(element->element, A_STANDOUT);
	w = 8.0;
	h = 4.0;

	for (deg = 0; deg < 360.0f; deg += 1.0f) {
		x = w + (int)(w * cos(DEG_TO_RAD(deg)));
		y = h + (int)(h * sin(DEG_TO_RAD(deg)));

		mvwaddch(element->element, y, x, 'O');
	}
}

static void draw_x(Grid *element)
{
	int i;
	int xpos, ypos;

	wattrset(element->element, A_STANDOUT);

	for (i = 1; i < (ELEMENT_HT - 1); i++) {
		ypos = i;
		xpos = i*2;
		mvwaddch(element->element, ypos, xpos, 'X');
		mvwaddch(element->element, ypos, xpos+1, 'X');
		mvwaddch(element->element, ypos, ELEMENT_WD-3-xpos, 'X');
		mvwaddch(element->element, ypos, ELEMENT_WD-4-xpos, 'X');
	}

}

static void draw_grid(ttt *t)
{
	int i, j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			t->grid[i][j].element = derwin(t->board,
						       ELEMENT_HT,
						       ELEMENT_WD,
						       i*ELEMENT_HT,
						       j*ELEMENT_WD);
			t->grid[i][j].type = ELEMENT_TYPE_NONE;
			if (i == 0 && j == 0)
				wattrset(t->grid[i][j].element, A_REVERSE);
			_window_draw_box(t->grid[i][j].element
					 , 0, 0, ELEMENT_HT, ELEMENT_WD,
					 A_NORMAL, A_NORMAL);
			wnoutrefresh(t->grid[i][j].element);
		}
	}
}

static void update_grid(ttt *t,
			int selected_x,
			int selected_y)
{
	int i, j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			t->grid[i][j].element = derwin(t->board,
						       ELEMENT_HT,
						       ELEMENT_WD,
						       i*ELEMENT_HT,
						       j*ELEMENT_WD);

			if (i == selected_x && j == selected_y) {
				wattrset(t->grid[i][j].element, A_REVERSE);
			} else {
				wattrset(t->grid[i][j].element, 0);
			}

			_window_draw_box(t->grid[i][j].element
					 , 0, 0, ELEMENT_HT, ELEMENT_WD,
					 A_NORMAL, A_NORMAL);

			if (t->grid[i][j].type == ELEMENT_TYPE_X)
				draw_x(&t->grid[i][j]);
			else if (t->grid[i][j].type == ELEMENT_TYPE_O)
				draw_o(&t->grid[i][j]);

			wnoutrefresh(t->grid[i][j].element);
		}
	}
}

static void draw_board(ttt *t)
{
	int xpos, ypos;
	int height, width;

	_window_clear(t);

	/* The border */
	/*
	height = 33;
	width = 65;
	xpos = (getmaxx(stdscr) - width) / 2;
	ypos = (getmaxy(stdscr) - height) / 2;

	t->border = newwin(height, width, ypos, xpos);
	keypad(t->border, TRUE);
	_window_draw_box(t->border, 0, 0, height, width, A_BOLD, A_NORMAL);

	wattrset(t->border, A_NORMAL);
	wbkgdset(t->border, A_NORMAL & A_COLOR);

	touchwin(t->border);
	wnoutrefresh(t->border);
	*/

	/* the board outline */
	height = 30;
	width = 60;
	xpos = (getmaxx(stdscr) - width) / 2;
	ypos = (getmaxy(stdscr) - height) / 2;

	t->board = newwin(height, width, ypos, xpos);
	keypad(t->board, TRUE);

	wattrset(t->board, A_NORMAL);
	wbkgdset(t->board, A_NORMAL & A_COLOR);

	touchwin(t->board);
	wnoutrefresh(t->board);

	/* the grid */
	draw_grid(t);

	wnoutrefresh(t->board);
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

static void game_loop(ttt *t)
{
	int ch = 0;
	int cur_x_pos = 0;
	int cur_y_pos = 0;

	nodelay(stdscr, FALSE);

	do {

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
				t->grid[cur_x_pos][cur_y_pos].type = ELEMENT_TYPE_O;
				draw_o(&t->grid[cur_x_pos][cur_y_pos]);
			}
			break;
		case 'x':
		case 'X':
			if (t->grid[cur_x_pos][cur_y_pos].type == ELEMENT_TYPE_NONE) {
				t->grid[cur_x_pos][cur_y_pos].type = ELEMENT_TYPE_X;
				draw_x(&t->grid[cur_x_pos][cur_y_pos]);
			}
			break;
		case '\n':
			break;
		default:
			continue;
		}

		update_grid(t, cur_x_pos, cur_y_pos);
		refresh();

	} while (1);

	nodelay(stdscr, FALSE);
}


int main(int argc, char **argv)
{
	ttt t;

	parse_args(basename(argv[0]), argc, argv);

	signal(SIGINT, termination_handler);

	if (screen_init() != 0) {
		exit(EXIT_FAILURE);
	}

	t.title = "Tic-Tac-Toe";

	draw_board(&t);
	refresh();

	game_loop(&t);

	screen_fin();

	exit(EXIT_SUCCESS);
}
