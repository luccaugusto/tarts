/* INCLUDES */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "pie_chart.h"
#include "bar_chart.h"
#include "line_chart.h"
#include "utils.h"

//TODO: Fix static assert
//static_assert(HEIGHT % 2 == 0, "Display height should be divisible by 2, if not pie chart compression won't work. See 'print_pie()' function");

/* GLOBAL VARIABLES */
static char chart[HEIGHT * WIDTH];

/* exits in case a chart can't be plotted */
void
rotten_tarts(PlotStatus status)
{
	char *err_msg;
	switch (status) {
		case PLOT_OK:
			//OK status but error caught, something weird happened
			err_msg = "Something weird happened, exiting.";
			break;
		case ERR_CIRCLE_TOO_BIG:
			err_msg = "Pie chart doesn't fit on screen with that scale, consider shrinking it";
			break;
		default:
			err_msg = "Error";
	}
	fprintf(stderr, "%s\n", err_msg);
	exit(status);
}

void
build_frame() {
	if (! FRAMED)
		return;

	for (int i=0; i < HEIGHT; ++i) {
		/* build border horizontally */
		if (i == HEIGHT-1 || i == 0) {
			chart[i * WIDTH] = FRAME_CORNER;
			int j=1;
			for (; j < WIDTH-1; ++j)
				chart[i * WIDTH + j] = FRAME_BORDER_H;
			chart [i * WIDTH + j] = FRAME_CORNER;
		}
		/* build border vertically */
		else {
			chart [i * WIDTH + 0] = FRAME_BORDER_V;
			chart [i * WIDTH + WIDTH - 1] = FRAME_BORDER_V;
		}

	}
}

void
show(char *title) {
	/* print title */
	strncpy(&chart[1], title, strlen(title));
	/* print chart */
	for (int i=0; i < HEIGHT; ++i)
		printf("%.*s\n", WIDTH, &chart[i * WIDTH] );
}

void
clear_screen()
{
	char *p = chart;
	int n = HEIGHT * WIDTH;

	while (n--) *p++ = BLANK;
}

int
main()
{
	int c;
	PlotStatus status;
	int x_offset = 2;
	float scale = (float)(HEIGHT) / (float)HEIGHT;
	Bar *b1 = new_bar(25, "lucca");
	Bar *b2 = new_bar(5, "luquinha");
	Bar *b3 = new_bar(50, "luccão");
	Pie *p = new_pie(10,HEIGHT/2, 14, 50);
	float series[2] = {2.5, 22.0};
	Line *l = new_line(series, "teste", 2);

	(void)b1;
	(void)b2;
	(void)b3;
	(void)p;
	(void)x_offset;
	//(void)l;

	do {
		clear_screen();

		print_bar(b1, chart, x_offset, scale);
		x_offset += (strlen(bar_get_name(b1)) + 1);

		print_bar(b2, chart, x_offset, scale);
		x_offset += (strlen(bar_get_name(b2)) + 1);

		print_bar(b3, chart, x_offset, scale);

		if (x_offset >= WIDTH)
			x_offset = 2;

		/*
		print_line_chart(l, chart, scale);

		if ((status = print_pie(p, chart, scale)))
			rotten_tarts(status);
			*/

		build_frame();
		show("TEST");
	} while ((c = getchar()) != 'q');

	return 0;
}
