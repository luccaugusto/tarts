#include <stdlib.h>
#include <ncurses.h>

#include "./utils.h"
#include "./constants.h"
#include "./canvas.h"

struct ChartFunctionTuple {
	void *chart;
	PlotFunction tart_function;
};

struct Tart {
	int chart_count;
	struct ChartFunctionTuple chart_list[MAX_CHARTS];
	Canvas *canvas;
	WINDOW *window;
};

char *
rotten_tarts(PlotStatus status)
{
	char *err_msg;
	switch (status) {
		case ERR_CIRCLE_TOO_BIG:
			err_msg = "Pie chart doesn't fit on canvas with that scale, consider shrinking it";
			break;
		case ERR_LINE_OUT:
			err_msg = "Line chart doesn't fit on canvas with that scale, consider shrinking it";
			break;
		default:
			err_msg = "Error";
	}

	return err_msg;
}

void
bake(struct Tart *tart)
{
	char *err_msg;
	(void)err_msg;
	canvas_clear(tart->canvas);

	if (tart->chart_count == 0) {
		mvwprintw(
			tart->window,
			canvas_get_height(tart->canvas)/2,
			canvas_get_width(tart->canvas)/4,
			"No charts to tart."
		);
		return;
	}

	for (int i=0; i<tart->chart_count; ++i) {
		PlotStatus status = tart->chart_list[i].tart_function(
				tart->chart_list[i].chart,
				canvas_get_dimentions(tart->canvas),
				canvas_get_canvas(tart->canvas),
				canvas_get_colors_fg(tart->canvas)
				);

		/* TODO: Make error message pop in a nice way, like the prompt_get_note from anote */
		if (status != PLOT_OK) {
			err_msg = rotten_tarts(status);
			canvas_set_scale(tart->canvas, canvas_get_scale(tart->canvas) - SCALE_INCREMENT);
			break;
		}
	}
	show_canvas(tart->canvas, tart->window);
}

struct Tart *
new_tart(Canvas *canvas, WINDOW *w)
{
	struct Tart *t = malloc(sizeof(struct Tart));
	t->canvas = canvas;
	t->window = w;

	return t;
}

void
tart_set_canvas(struct Tart *t, Canvas *canvas)
{
	t->canvas = canvas;
}

int
tart_add_chart(struct Tart *t, void *chart, PlotFunction tart_function)
{
	if (t->chart_count >= MAX_CHARTS)
		return 0;

	struct ChartFunctionTuple cft;
	cft.chart = chart;
	cft.tart_function = tart_function;

	t->chart_list[t->chart_count++] = cft;
	return 1;
}

void
destroy_tart(struct Tart *t)
{
	for (int i=0; i<t->chart_count; ++i) {
	}
	free(t);
}

int
tart_get_chart_count(struct Tart *t)
{
	return t->chart_count;
}

void
tart_set_window(struct Tart *t, WINDOW *w)
{
	t->window = w;
}
