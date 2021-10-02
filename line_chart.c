#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "constants.h"
#include "canvas.h"
#include "line_chart.h"
#include "utils.h"

struct LineChart {
	float *points;
	char name[MAX_NAME_LENGTH];
	int count_points;
};

struct LineChart *
new_line(float *points, char *name, int width, int count_points)
{
	int name_len = (strlen(name) > MAX_NAME_LENGTH) ?
		MAX_NAME_LENGTH : strlen(name);

	count_points = (count_points > width - PADDING) ?
		width - PADDING : count_points;

	Line *l = malloc(sizeof(Line));
	l->count_points = count_points;
	strncpy(l->name, name, name_len);
	l->points = malloc(sizeof(float) * count_points);
	memcpy(l->points, points, count_points * sizeof(float));

	return l;
}

float *
line_get_points(Line *l)
{
	return l->points;
}

char *
line_get_name(Line *l)
{
	return l->name;
}

int
line_get_count_points(Line *l)
{
	return l->count_points;
}

/*
 * Draws a line using Bresenham's algorithm
 */
void
draw_line(Canvas *canvas, int prev_x, int prev_y, int cur_x, int cur_y)
{
	int x, y, dx, dy, i, incrx, incry, const1, const2, p;
	int y_offset = 0;
	int width = canvas_get_width(canvas);
	char lineblock = HORIZONTAL;
	char *canvas_screen = canvas_get_canvas(canvas);
	dx = cur_x - prev_x;
	dy = cur_y - prev_y;

	if (dx >= 0) {
		incrx = 1;
	} else {
		incrx = -1;
		dx = -dx;
	}

	if (dy >= 0) {
		incry = 1;
	} else {
		incry = -1;
		dy = -dy;
	}

	x = prev_x;
	y = prev_y;

	/* line going down */
	if (prev_y < cur_y) {
		y_offset = 0;
		lineblock = LINE_DOWN;
	}

	/* line going up */
	else {
		y_offset = 1;
		lineblock = LINE_UP;
	}

	if (dy < dx) {
		p = 2 * dy - dx;
		const1 = 2 * dy;
		const2 = 2 * (dy - dx);
		for(i = 0; i < dx; i++) {
			x += incrx;
			if (p < 0) {
				p += const1;
				canvas_screen[y*width + x] = HORIZONTAL;
			} else {
				y += incry;
				p += const2;
				canvas_screen[(y + y_offset)*width + x] = lineblock;
			}
		}
	} else {
		p = 2 * dx - dy;
		const1 = 2 * dx;
		const2 = 2 * (dx - dy);
		for(i = 0; i < dy; i++) {
			y += incry;
			if (p < 0) {
				p += const1;
				canvas_screen[y*width + x] = HORIZONTAL;
			} else {
				x += incrx;
				p += const2;
				canvas_screen[(y+1)*width + x] = LINE_DOWN;
			}
		}
	}
}

void
print_line_chart(Line *l, Canvas *canvas, float scale)
{
	char *point_str;
	char *canvas_screen = canvas_get_canvas(canvas);
	float *points = line_get_points(l);
	int width = canvas_get_width(canvas);
	int height = canvas_get_height(canvas);
	int count_points = line_get_count_points(l);
	int spacing = (width - PADDING*2) / (count_points - 1);
	float scaled_point;
	int prev_x = -1;
	int prev_y = -1;

	/* pad both sides */
	int x = PADDING/2;
	for (int i=0; i<count_points; ++i) {
		scaled_point = height - (points[i] * scale);
		canvas_screen[(int)(scaled_point * width) + x] = HORIZONTAL;

		/* draw line from first point */
		if (prev_x >= 0 && prev_y >= 0)
			draw_line(canvas, prev_x, prev_y, x, scaled_point);

		prev_x = x;
		prev_y = scaled_point;

		/* put value above point */
		point_str = float2str(points[i]);
		strncpy(&canvas_screen[((int)scaled_point-1) * width + x], point_str, strlen(point_str));
		free(point_str);

		x+=spacing;
	}
}
