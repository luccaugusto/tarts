#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <panel.h>

#include "./utils.h"
#include "./constants.h"
#include "./canvas.h"
#include "./tart.h"
#include "./prompt.h"
#include "./line_chart.h"
#include "pie_chart.h"

struct GenericChart {
	void *chart;
	PlotFunction tart_function;
	ChartType type;
	short chart_color;
};

struct Tart {
	int chart_count;
	struct GenericChart chart_list[MAX_CHARTS];
	Canvas *canvas;
	WINDOW *window;
};

PlotStatus
bake(struct Tart *tart)
{
	canvas_clear(tart->canvas);
	PlotStatus status;

	/* if (canvas_get_show_scale(tart->canvas))
		show_scale(tart->canvas); */

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
tart_add_chart(struct Tart *t, void *chart, PlotFunction tart_function, ChartType type)
{
	if (t->chart_count >= MAX_CHARTS)
		return 0;

	struct GenericChart cft;
	cft.chart = chart;
	cft.tart_function = tart_function;
	cft.type = type;

	t->chart_list[t->chart_count++] = cft;
	return 1;
}

void
destroy_tart(struct Tart *t)
{
	for (int i=0; i<t->chart_count; ++i) {
		free(t->chart_list[i].chart);
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

/* TODO */
void delete_chart(){return;}
void add_new_chart(){return;}
void show_commands_panel(){return;}

void
show_chart_stats(struct Tart *t)
{
	double avg = 0, min = 0, max = 0, total = 0;
	char name[MAX_NAME_LENGTH];
	char stats[MAX_NAME_LENGTH];
	char *title = "Stats for chart ";
	char *label = NULL;

	for (int i=0; i<t->chart_count; ++i) {
		switch (t->chart_list[i].type) {
			case LINE_CHART:; /* empty statement because language requires */
				Line *l = (Line *)t->chart_list[i].chart;
				double *points = line_get_points(l);
				int count_points = line_get_count_points(l);
				total = 0;
				min = max = points[0];
				for (int j=0; j<count_points; ++j) {
					total+=points[j];
					if (points[j] < min)
						min = points[j];

					if (points[j] > max)
						max = points[j];
				}
				avg = total/count_points;
				strncpy(name, line_get_name(l), MAX_NAME_LENGTH);
				label = malloc(sizeof(char) * (strlen(name) + strlen(title)+1));
				strncpy(label, title, strlen(title)+1);
				strncpy(&label[strlen(title)], name, strlen(name));
				break;
			case PIE_CHART:;
				Pie *p = (Pie *)t->chart_list[i].chart;
				int count_portions = get_portion_count(p);
				Portion *portion = get_portion_by_index(p, 0);
				if (portion == NULL) {
					alert(tart_get_canvas(t), "No portions in pie chart", "Error:", TRUE);
					return;
				}
				total = 0;
				min = max = portion_get_value(portion);
				for (int j=0; j<count_portions; ++j) {
					portion = get_portion_by_index(p, j);
					double value = portion_get_value(portion);
					total += value;
					if (value < min) min = value;
					if (value > max) max = value;
				}
				avg = total/count_portions;
				label = malloc(sizeof(char) * (strlen("Pie #1") + strlen(title)));
				strncpy(label, title, strlen(title)+1);
				strncpy(&label[strlen(title)], name, strlen(name)+1);
				break;
			case BAR_CHART:
				break;
			default:
				break;
		}
		snprintf(stats, MAX_NAME_LENGTH, "AVG: %.2f, TOTAL: %.2f, MIN: %.2f, MAX: %2.f", avg, total, min, max);
		alert(tart_get_canvas(t), stats, label, TRUE);
		free(label);
	}
}

char *
rotten_tarts(PlotStatus status)
{
	char *err_msg;
	switch (status) {
		case ERR_CIRCLE_TOO_BIG:
			err_msg = "Pie chart doesn't fit on canvas with that scale";
			break;
		case ERR_LINE_OUT:
			err_msg = "Line chart doesn't fit on canvas with that scale";
			break;
		case ERR_BAR_TOO_BIG:
			err_msg = "Bar chart doesn't fit on canvas with that scale";
			break;
		default:
			err_msg = "Error";
	}

	char *err_msg_copy = malloc(sizeof(char) * (strlen(err_msg) + 1));
	strncpy(err_msg_copy, err_msg, strlen(err_msg));

	return err_msg_copy;
}

void
execution_loop(Tart *tart, WINDOW *tarts_w, WINDOW *footer_w)
{
	int c = BLANK;
	char *err_msg;
	PlotStatus status = PLOT_OK;
	Canvas *canvas = tart_get_canvas(tart);
	do {
		switch (c) {
			case '?':
				show_commands_panel();
				break;
			case 'i':
				canvas_zoom(canvas, ZOOM_IN);
				break;
			case 'd':
				canvas_zoom(canvas, ZOOM_OUT);
				break;
			case 'n':
				add_new_chart();
				break;
			case 'D':
				delete_chart();
				break;
			case 'r':
				show_chart_stats(tart);
				break;
			default:
				//NOP
				break;
		};
		if ((status = bake(tart)) != PLOT_OK) {
			err_msg = rotten_tarts(status);
			canvas_zoom(canvas, canvas_get_scale(canvas) < 1 ? ZOOM_IN : ZOOM_OUT);
			alert(canvas, err_msg, "Error:", TRUE);
			free(err_msg);
		}
		box(tarts_w,0,0);
		box(footer_w,0,0);
		wrefresh(tarts_w);
		wrefresh(footer_w);
		doupdate();
	} while ((c = wgetch(tarts_w)) != 'q');
}
