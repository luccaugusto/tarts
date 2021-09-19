#ifndef LINE_CHART_H
#define LINE_CHART_H

enum LineBlocks {
	HORIZONTAL = '_',
	LINE_UP    = '/',
	LINE_DOWN  = '\\',

};

typedef struct LineChart Line;

struct LineChart *new_line(float *points, char *name, int points_len);
float *line_get_points(Line *l);
char *line_get_name(Line *l);
void print_line_chart(Line *l, char* chart, float scale);
int line_get_points_len(Line *l);

#endif
