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

struct _ttt {
	WINDOW *board;
	WINDOW *title;
	WINDOW *msg;
};

typedef struct _ttt ttt;

/* screen size*/
static void get_screensize_using_termcap(short *widthp, short *heightp)
{
        *widthp = 0;
        *heightp = 0;

        if ((*heightp = (short)tgetnum("li")) == -1) {
                fprintf(stderr, "Failed to get termcap entry for lines\n");
                return;
        }

        if ((*widthp = (short)tgetnum("co")) == -1) {
                fprintf(stderr, "Failed to get termcap entry for columns\n");
                return;
        }
}

static void get_screensize_using_ioctl(int *widthp, int *heightp)
{
#ifdef TIOCGWINSZ
        struct winsize size;

        *widthp = 0;
        *heightp = 0;

        if (ioctl(0, TIOCGWINSZ, &size) < 0)
                return;

        *widthp = size.ws_col;
        *heightp = size.ws_row;
#else
        *widthp = 0;
        *heightp = 0;
#endif
}


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
        printf("Left Arrow  :  Move left.\n");
        printf("Right Arrow :  Move right.\n");
        printf("Up Arrow    :  Move up.\n");
        printf("Down Arrow  :  Move down.\n");
        printf("Enter       :  Select entry.\n");
        printf("p/P         :  Pause the game.\n");
        printf("q/Q/C-c/Esc :  Quit the game.\n");

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

static void _window_draw_box(WINDOW *win,
			     int y, int x,
			     int height, int width,
			     chtype box, chtype border)
{
	int i, j;

	wattrset(win, 0);
	for (i = 0; i < height; i++) {
		wmove(win, y + i, x);
		for (j = 0; j < width -2; j++)
			if (!i && !j)
				waddch(win, border | ACS_ULCORNER);
			else if (i == height - 1 && !j)
				waddch(win, border | ACS_LLCORNER);
			else if (!i && j == width - 3)
				waddch(win, box | ACS_URCORNER);
			else if (i == height - 1 && j == width - 1)
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

static void draw_board(ttt *t)
{
	//WINDOW *win = _window_draw_box();
}

int main(int argc, char **argv)
{
	ttt t;

	parse_args(basename(argv[0]), argc, argv);

	signal(SIGINT, termination_handler);

	if (screen_init() != 0) {
		perror("Error initialising ncurses");
		exit(EXIT_FAILURE);
	}

	t.board = newwin(10, 10, 20, 20);
	box(t.board, '|', '-');
	touchwin(t.board);
	wrefresh(t.board);
	keypad(t.board, TRUE);

	draw_board(&t);

	getchar();

	screen_fin();

	exit(EXIT_SUCCESS);
}

