#ifndef PLOT_H
#define PLOT_H

typedef struct Plot Plot;

void execute_plot(Plot *plot);
Plot *new_plot(Canvas *canvas);
void plot_set_canvas(struct Plot *p, Canvas *canvas);
void plot_add_chart(struct Plot *p, void *chart, PlotFunction plot_function);

#endif
