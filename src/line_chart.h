#ifndef LINE_CHART_H
#define LINE_CHART_H

enum LineBlocks {
	HORIZONTAL = '_',
	LINE_UP    = '/',
	LINE_DOWN  = '\\',
	LINE_POINT = 'o',
};

typedef struct LineChart Line;

struct LineChart *new_line(double *points, char *name, int width, int points_len);
double *line_get_points(Line *l);
char *line_get_name(Line *l);
PlotStatus print_line_chart(void *line, struct Dimentions *dimentions, char *canvas_screen, Color *canvas_colors);
int line_get_count_points(Line *l);
void line_set_color(Line *l, Color c);
Color line_get_color(Line *l);
int line_get_stats(Line *l, double *min, double *max, double *avg, double *total);

#endif
