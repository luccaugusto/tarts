#ifndef PLOT_H
#define PLOT_H

typedef struct Plot Plot;

void execute_plot(Plot *plot, WINDOW *w);
Plot *new_plot(Canvas *canvas);
void plot_set_canvas(struct Plot *p, Canvas *canvas);
int plot_add_chart(struct Plot *p, void *chart, PlotFunction plot_function);
void destroy_plot(struct Plot *p);
int plot_get_chart_count(struct Plot *p);

#endif
