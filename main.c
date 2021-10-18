/* INCLUDES */
#include <argp.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <errno.h>

#include "./utils.h"
#include "./constants.h"
#include "./canvas.h"
#include "./pie_chart.h"
#include "./bar_chart.h"
#include "./line_chart.h"
#include "./tart.h"

/* GLOBAL VARIABLES  */
int max_height;
int max_width;
double scale = 1;
char *err_msg = "\0";
WINDOW *tarts_w;
WINDOW *footer_w;
short color_list[] = {
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE,
};
int color_used = 0;

const char *argp_program_version = VERSION;
const char *argp_program_bug_address = MANTAINER_EMAIL;
static char doc[] = "Plot delicious Charts on the Terminal with T(CH)ARTS";
static char args_doc[] = "[tarts]...";
static struct argp_option options[] = {
	/* arg name, flag,   arg value   ,is optional,       description         */
	{ "type"        , 't' ,  "chart type" ,     0     , "[bar/pie/line]"           },
	{ "file"        , 'f' ,  "filename"   ,     0     , "Read charts from file"    },
	{ "label"       , 'l' , "chart label" ,     0     , "Give chart a label"       },
	{ "values"      , 'v' , "value list"  ,     0     , "List of values for chart" },
	{ "interactive" , 'i' ,       0       ,     0     , "Disable interactive mode" },
	{ 0 }
};

/* TYPES */
struct arguments {
	enum {NONE, PIECHART, LINECHART, BARCHART} charts[MAX_CHARTS];
	int charts_count;
	char *labels[MAX_CHARTS];
	int labels_count;
	double values[MAX_CHARTS];
	int values_count;
	int interactive;
};

/* TODO */
void delete_chart(){return;}
void add_new_chart(){return;}
void show_commands_panel(){return;}

/* FUNCTION DEFINITIONS */
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
housekeeping(Canvas *s, Tart *t)
{
	delwin(tarts_w);
	delwin(footer_w);
	if (s != NULL)
		destroy_canvas(s);
	destroy_tart(t);
	show_cursor(1);
	echo();
	endwin();
}

void
show_footer_info(void)
{
	mvwprintw(footer_w, 1, 1, "?: Help | q: Quit Tarts | i: increase scale | d: decrease scale");
}

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
		case 'l': arguments->labels[arguments->labels_count++] = arg;
				  break;
		case 't': switch (arg[0]) {
					  case 'p': arguments->charts[arguments->charts_count++] = PIECHART;
								break;
					  case 'l': arguments->charts[arguments->charts_count++] = LINECHART;
								break;
					  default:
					  case 'b': arguments->charts[arguments->charts_count++] = BARCHART;
								break;
				  }
				  break;
		case 'v': arguments->values[arguments->values_count++] = str2double(arg);
				  break;
		case 'i': arguments->interactive = 0;
		case ARGP_KEY_ARG:
			return 0;
		default:
			return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

/* Plot charts specified on command line */
void
prepare_cmd_line_tarts(struct arguments *arguments, double *max_value, int tarts_height, Tart *tart)
{
	for (int i=0; i<arguments->charts_count; ++i) {
		if (arguments->values[i] > *max_value)
			*max_value = arguments->values[i];
		switch (arguments->charts[i]) {
			case PIECHART:
				break;
			case LINECHART:
				break;
			case BARCHART: ;
						   Bar *b = new_bar(arguments->values[i], arguments->labels[i]);
						   bar_set_color(b, color_list[color_used++]);
						   tart_add_chart(tart, b, print_bar_chart);
						   break;
			default:
			case NONE:
						   break;
		}
	}

	canvas_set_scale(tart_get_canvas(tart), (scale = (double)(tarts_height - PADDING) / *max_value));
}

int
main(int argc, char *argv[])
{
	struct arguments arguments;
	arguments.interactive = 1;
	arguments.values_count = 0;
	arguments.charts_count = 0;
	arguments.labels_count = 0;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

	/* Begin */
	init_tart();

	int c = BLANK;
	int did_add = 0;
	int tarts_width = max_width;
	int tarts_height = max_height - FOOTER_HEIGHT;
	double max_value = 0;
	PlotStatus status = PLOT_OK;
	/* Borders ocupy one char on left, right, top and bottom */
	Canvas *canvas = new_canvas(tarts_height-2, tarts_width-2);
	canvas_set_scale(canvas, scale);

	tarts_w = create_new_win(tarts_height, tarts_width, 0, 0);
	footer_w = create_new_win(FOOTER_HEIGHT, max_width, tarts_height, 0);

	Tart *tart = new_tart(canvas, tarts_w);

	show_footer_info();

	prepare_cmd_line_tarts(&arguments, &max_value, tarts_height, tart);

	if (! arguments.interactive) {
		if (tart_get_chart_count(tart) > 0) {
			bake(tart);
			housekeeping(NULL, tart);
			print_canvas(canvas);
			destroy_canvas(canvas);
		} else {
			housekeeping(canvas,tart);
			printf("No charts to tart.\n");
		}
		return 0;
	}

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
		bake(tart);
		wrefresh(tarts_w);
		wrefresh(footer_w);
		doupdate();
	} while ((c = wgetch(tarts_w)) != 'q');

	housekeeping(canvas, tart);
	return status;
}
