/* INCLUDES */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <errno.h>

#include "constants.h"
#include "canvas.h"
#include "pie_chart.h"
#include "bar_chart.h"
#include "line_chart.h"
#include "utils.h"
#include "plot.h"

int max_height;
int max_width;
double scale;
char *err_msg = "\0";
WINDOW *tarts_w;

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
housekeeping(Canvas *s)
{
	delwin(tarts_w);
	destroy_canvas(s);
	show_cursor(1);
	echo();
	endwin();
}

int
main()
{
	init_tart();

	int c = BLANK;
	PlotStatus status = PLOT_OK;
	int x_offset = 2;
	scale = (double)max_height / (double)max_height;

	Bar *b1 = new_bar(25, "lucca");
	bar_set_color(b1, COLOR_YELLOW);
	Bar *b2 = new_bar(5, "luquinha");
	Bar *b3 = new_bar(50, "luccão");
	(void)b1;
	(void)b2;
	(void)b3;
	Pie *p = new_pie(10,max_height/2, 15, 50);
	Portion *portion_1 = new_portion(20, "metade 1", COLOR_GREEN);
	Portion *portion_2 = new_portion(20, "metade 2", COLOR_BLUE);
	Portion *portion_3 = new_portion(20, "metade 3", COLOR_RED);
	Portion *portion_4 = new_portion(20, "metade 3", COLOR_WHITE);
	Portion *portion_5 = new_portion(20, "metade 3", COLOR_YELLOW);
	pie_push_portion(p, portion_1);
	pie_push_portion(p, portion_2);
	pie_push_portion(p, portion_3);
	pie_push_portion(p, portion_4);
	pie_push_portion(p, portion_5);
	/*
	*/

	double series[2] = {2.5, 22.0};
	double series2[4] = {3.5, 17.0, 10.0, 8.9};
	Canvas *canvas = new_canvas(max_height-2, max_width-2);
	Plot *plot = new_plot(canvas);
	Line *l = new_line(series, "teste", canvas_get_width(canvas), 2);
	line_set_color(l, COLOR_BLUE);
	Line *l2 = new_line(series2, "teste", canvas_get_width(canvas), 4);
	line_set_color(l2, COLOR_GREEN);

	tarts_w = create_new_win(max_height, max_width, 0, 0);

	(void)p;
	(void)x_offset;
	(void)l;

	do {
		if (c == 'i')
			scale += SCALE_INCREMENT;
		else if (c == 'd')
			scale -= SCALE_INCREMENT;
		canvas_set_scale(canvas, scale);

		canvas_clear(canvas);

		plot_setup(plot, print_bar_chart, b1);
		execute_plot(plot);
		/*
		plot_setup(plot, print_line_chart, l2);
		execute_plot(plot);
		*/

		show_canvas(canvas, tarts_w);
		wrefresh(tarts_w);
		doupdate();
	} while ((c = getchar()) != 'q');
	housekeeping(canvas);

	return status;
}
