/* INCLUDES */
#include <argp.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <errno.h>

#include "utils.h"
#include "constants.h"
#include "canvas.h"
#include "pie_chart.h"
#include "bar_chart.h"
#include "line_chart.h"
#include "plot.h"

/* GLOBAL VARIABLES  */
int max_height;
int max_width;
double scale = 1;
char *err_msg = "\0";
WINDOW *tarts_w;
WINDOW *footer_w;

const char *argp_program_version = VERSION;
const char *argp_program_bug_address = MANTAINER_EMAIL;
static char doc[] = "Plot delicious Charts on the Terminal with T(CH)ARTS";
static char args_doc[] = "[tarts]...";
static struct argp_option options[] = {
	/* arg name, flag, arg value, is optional, description */
    { "line"   , 'l' ,     0     ,     0     , "Plot line chart."   },
    { "pie"    , 'p' ,     0     ,     0     , "Plot pie chart."    },
    { "bar"    , 'b' ,     0     ,     0     , "Plot bar chart."    },
    { "file"   , 'f' , "filename",     0     , "Read charts from file" },
    { 0 }
};

struct arguments {
    enum { PIE, LINE, BAR } chart;
};

/* TODO */
void delete_chart(){return;}
void add_new_chart(){return;}
void show_commands_panel(){return;}

void
show_cursor(int show)
{
	if (show)
		curs_set(1);
	else
		curs_set(0);
}

WINDOW *
create_new_win(int height, int width, int start_y, int start_x)
{
	WINDOW *local_win;

	local_win = newwin(height, width, start_y, start_x);

	/* use default caracters for borders */
	box(local_win, 0 , 0);
	wrefresh(local_win);

	return local_win;
}

void
init_tart(void)
{
	/* start ncurses     */
	if (initscr() == NULL) {
		perror(strerror(errno));
		exit(errno);
	}

	 if (has_colors() == FALSE) {
        endwin();
        puts("This program requires a terminal with color support");
        exit(1);
    }

	raw();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	show_cursor(0);
	getmaxyx(stdscr,max_height,max_width);

    start_color();
    init_colorpairs();
}

void
housekeeping(Canvas *s, Plot *p)
{
	delwin(tarts_w);
	delwin(footer_w);
	destroy_canvas(s);
	destroy_plot(p);
	show_cursor(1);
	echo();
	endwin();
}

void
show_footer_info(void)
{
	mvwprintw(footer_w, 1, 1, "? to show help");
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
		case 'l': arguments->chart = LINE; break;
		case 'p': arguments->chart = PIE; break;
		case 'b': arguments->chart = BAR; break;
		case ARGP_KEY_ARG: return 0;
		default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int
main(int argc, char *argv[])
{
	struct arguments arguments;

    arguments.chart = LINE;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

	/* Begin */
	init_tart();

	int c = BLANK;
	int tarts_width = max_width;
	int tarts_height = max_height - FOOTER_HEIGHT;
	PlotStatus status = PLOT_OK;
	/* Borders ocupy one char on left, right, top and bottom */
	Canvas *canvas = new_canvas(tarts_height-2, tarts_width-2);
	canvas_set_scale(canvas, scale);
	Plot *plot = new_plot(canvas);

	tarts_w = create_new_win(tarts_height, tarts_width, 0, 0);
	footer_w = create_new_win(FOOTER_HEIGHT, max_width, tarts_height, 0);

	show_footer_info();

	/* TODO: move execution loop to a separate function */
	do {
		switch (c) {
			case '?':
				show_commands_panel();
				break;
			case 'i':
				scale += SCALE_INCREMENT;
				canvas_set_scale(canvas, scale);
				break;
			case 'd':
				scale -= SCALE_INCREMENT;
				canvas_set_scale(canvas, scale);
				break;
			case 'n':
				add_new_chart();
				break;
			case 'D':
				delete_chart();
				break;
			default:
				//NOP
				break;
		};
		execute_plot(plot, tarts_w);
		wrefresh(tarts_w);
		wrefresh(footer_w);
		doupdate();
	} while ((c = wgetch(tarts_w)) != 'q');

	housekeeping(canvas, plot);
	return status;
}
