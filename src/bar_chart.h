#ifndef BAR_CHART_H
#define BAR_CHART_H

#include "constants.h"
#define MAX_BARS ((WIDTH - MAX_NAME_LENGTH) / 3)
#define BARBLOCK '#'
#define BARBLOCK_HALF '='
#define BARBLOCK_QUARTER '.'

/* TYPES */
typedef struct BarChart Bar;

/* GLOBAL VARIABLES */
extern int global_x_offset;

/* FUNCTION PROTOTYPES */
double bar_get_number (Bar *series);
char *bar_get_name (Bar *series);
Color bar_get_color (Bar *series);
int bar_get_offset (Bar *series);
void bar_set_number (Bar *series, double number);
void bar_set_name (Bar *series, char *name);
void bar_set_color (Bar *series, Color color);
void bar_set_offset (Bar *series, int offset);
PlotStatus print_bar_chart(void *s, struct Dimentions *dimentions, char *canvas_screen, Color *canvas_colors);
Bar *new_bar (double number, char *name);
#endif
