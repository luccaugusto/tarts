#ifndef PLOT_H
#define PLOT_H

typedef struct Plot Plot;

void execute_plot(Plot *plot);
Plot *new_plot(Canvas *canvas);
void plot_setup(struct Plot *p, PlotStatus (*function)(void *, struct Dimentions *, char *, Color *), void *chart);
void plot_set_function(struct Plot *p, PlotStatus (*function)(void *, struct Dimentions *, char *, Color *));
void plot_set_canvas(struct Plot *p, Canvas *canvas);
void plot_set_chart(struct Plot *p, void *chart);

#endif
