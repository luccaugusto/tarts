#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "line_chart.h"
#include "utils.h"

struct LineChart {
	double *points;
	char name[MAX_NAME_LENGTH];
	int count_points;
	Color color;
};

struct LineChart *
new_line(double *points, char *name, int width, int count_points)
{
	int name_len = (strlen(name) > MAX_NAME_LENGTH) ?
		MAX_NAME_LENGTH : strlen(name);

	count_points = (count_points > width - PADDING) ?
		width - PADDING : count_points;

	Line *l = malloc(sizeof(Line));
	l->count_points = count_points;
	strncpy(l->name, name, name_len);
	l->points = malloc(sizeof(double) * count_points);
	memcpy(l->points, points, count_points * sizeof(double));

	return l;
}

double *
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

Color
line_get_color(Line *l)
{
	return l->color;
}

void
line_set_color(Line *l, Color c)
{
	l->color = c;
}

/*
 * Draws a line using Bresenham's algorithm
 */
void
draw_line(int width, char *canvas_screen, Color *canvas_colors, int prev_x, int prev_y, int cur_x, int cur_y, Color color)
{
	int x, y, dx, dy, i, incrx, incry, const1, const2, p;
	int y_offset = 0;
	char lineblock = HORIZONTAL;
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

	/* mark current and previous points */
	canvas_screen[cur_y * width + cur_x] = LINE_POINT;
	canvas_screen[prev_y * width + prev_x] = LINE_POINT;

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
				canvas_colors[y*width + x] = color;
			} else {
				y += incry;
				p += const2;
				canvas_screen[(y + y_offset)*width + x] = lineblock;
				canvas_colors[(y + y_offset)*width + x] = color;
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
				canvas_colors[y*width + x] = color;
			} else {
				x += incrx;
				p += const2;
				canvas_screen[(y+1)*width + x] = LINE_DOWN;
				canvas_colors[(y+1)*width + x] = color;
			}
		}
	}
}

int
print_line_chart(Line *l, int width, int height, char *canvas_screen, Color *canvas_colors, double scale)
{
	char *point_str;
	double *points = line_get_points(l);
	int count_points = line_get_count_points(l);
	int spacing = (width - PADDING*2) / (count_points - 1);
	double scaled_point;
	int prev_x = -1;
	int prev_y = -1;

	/* pad both sides */
	int x = PADDING/2;

	for (int i=0; i<count_points; ++i) {
		scaled_point = height - (points[i] * scale);

		if (scaled_point > (double)height || scaled_point < 0.0)
			return ERR_LINE_OUT;

		/* draw line from first point */
		if (prev_x >= 0 && prev_y >= 0)
			draw_line(width, canvas_screen, canvas_colors, prev_x, prev_y, x, scaled_point, l->color);

		prev_x = x;
		prev_y = scaled_point;

		/* put value above point and color it */
		point_str = double2str(points[i]);
		int y = (int)(scaled_point - 1);
		/* make sure point value is on a blank space,
		 * if there is no way, just plot it on top of point
		 * TODO: didn't work for some reason, make it work
		 */
		while (canvas_screen[y * width + x] != BLANK) {
			if (y < height / 2) {
				if (y - 1 <= 1)
					break;
				--y;
			} else {
				if (y + 1 >= height)
					break;
				++y;
			}
		}
		strncpy(&canvas_screen[y * width + x], point_str, strlen(point_str));

		int j = y * width + x;
		int k = j + strlen(point_str);
		for (; j < k; ++j)
			canvas_colors[j] = l->color;

		free(point_str);

		x+=spacing;
	}

	return PLOT_OK;
}
