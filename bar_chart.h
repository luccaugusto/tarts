#ifndef BAR_CHART_H
#define BAR_CHART_H

#define MAX_BARS ((WIDTH - MAX_NAME_LENGTH) / 3)
#define BARBLOCK '#'
#define BARBLOCK_HALF '='
#define BARBLOCK_QUARTER '.'

/* TYPES */
typedef struct BarChart Bar;

/* FUNCTION PROTOTYPES */
float bar_get_number (Bar *series);
char *bar_get_name (Bar *series);
Color bar_get_color (Bar *series);
void bar_set_number (Bar *series, float number);
void bar_set_name (Bar *series, char *name);
void bar_set_color (Bar *series, Color color);
void print_bar_chart(Bar *series, int width, int height, char *canvas_screen, Color *canvas_colors, float scale);
Bar *new_bar (float number, char *name);
#endif
