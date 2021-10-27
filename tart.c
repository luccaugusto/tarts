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

PlotStatus
bake(struct Tart *tart)
{
	canvas_clear(tart->canvas);
	PlotStatus status;

	if (canvas_get_show_scale(tart->canvas))
		show_scale(tart->canvas);

	if (tart->chart_count == 0) {
		mvwprintw(
			tart->window,
			canvas_get_height(tart->canvas)/2,
			canvas_get_width(tart->canvas)/4,
			"No charts to tart."
		);
		return PLOT_OK;
	}

	for (int i=0; i<tart->chart_count; ++i) {
		status = tart->chart_list[i].tart_function(
			tart->chart_list[i].chart,
			canvas_get_dimentions(tart->canvas),
			canvas_get_canvas(tart->canvas),
			canvas_get_colors_fg(tart->canvas)
		);

		if (status != PLOT_OK)
			break;
	}

	if (status == PLOT_OK)
		show_canvas(tart->canvas, tart->window);

	return status;
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

Canvas *
tart_get_canvas(struct Tart *t)
{
	return t->canvas;
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
