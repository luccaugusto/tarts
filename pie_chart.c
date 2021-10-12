#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "constants.h"
#include "pie_chart.h"
#include "utils.h"

struct Portion {
	int percentage;
	char *name;
	Color color;
};

struct PieChart {
	double center_x;
	double center_y;
	double radius;
	double total;
	struct Portion portion_stack[MAX_PORTIONS];
	int count_portions;
	Color color_by_angle_map[CIRCLE_DEGREE_COUNT];
	int color_map_color_count;
};

int
circle_fits(Pie *pie, int width, int height, double scale)
{
	double center_x = get_center_x(pie);
	double center_y = get_center_y(pie);
	double scaled_radius = get_radius(pie) * scale;
	double scaled_radius_h = scaled_radius/2 + 0.5;

	if (center_x + scaled_radius > width) {
		center_x = (center_x + scaled_radius - width);

		if (center_x - scaled_radius < 0)
			return ERR_CIRCLE_TOO_BIG;

		set_center_x(pie, center_x);
	}

	if (center_x - scaled_radius < 0) {
		center_x = center_x + -(center_x - scaled_radius);

		if (center_x + scaled_radius > width)
			return ERR_CIRCLE_TOO_BIG;

		set_center_x(pie, center_x);
	}

	/* y gets compressed so we can test with scaled_radius/2 */
	if (center_y + scaled_radius_h > height) {
		center_y = (center_y + scaled_radius - height);

		if (center_y - scaled_radius_h < 0)
			return ERR_CIRCLE_TOO_BIG;

		set_center_y(pie, center_y);
	}

	if (center_y - scaled_radius_h < 0) {
		center_y = -(center_y - scaled_radius);

		if (center_y + scaled_radius_h + 1 > height)
			return ERR_CIRCLE_TOO_BIG;

		set_center_y(pie, center_y);
	}

	return PLOT_OK;
}

/**
 * Prints the pie chart preserving circle aspect ratio
 * Compresses 2 rows into one so aspect ratio of circle is not deformed.
 * this is how it is rendered: (. represent blanks and * represents filled pixels)
 *
 * This pair of rows turn into the single row seen below
 * ROW 1 [.,*,.,*]
 * ROW 2 [.,.,*,*]
 *
 * ROW C [ ,",~,O]
 * these characters can be changed by the macros
 * BLANK, PIEBLOCK_TOP, PIEBLOCK_BOTTOM and PIEBLOCK_FULL respectively
 *  */
/* TODO: refactor this into a smaller function signature */
int
print_pie(Pie *pie, int width, int height, char *canvas_screen, Color *canvas_colors, double scale)
{
	int c, top, bottom, ret_code, length;
	/* check for screen boundaries */
	if ((ret_code = circle_fits(pie, width, height, scale)) != PLOT_OK)
		return ret_code;

	// bottom and top are indices b  t
	static char compression_table[2][2] = {
		/*t         0                  1     */
		/*b0 */{BLANK,           PIEBLOCK_TOP },
		/*b1 */{PIEBLOCK_BOTTOM, PIEBLOCK_FULL},
	};
	/* compute circle limits */
	double angle, radians, dx, dy, divisor;
	double center_x = get_center_x(pie);
	double center_y = get_center_y(pie);
	double scaled_radius = get_radius(pie) * scale;
	double left_x = center_x - scaled_radius;
	double right_x = center_x + scaled_radius;
	double top_y = center_y - scaled_radius;
	double bottom_y = center_y + scaled_radius;
	double radius_sqr = scaled_radius*scaled_radius;
	int labels_x_offset = (center_x < width/2) ? PIE_OFFSET : width - PIE_OFFSET - 10;
	int circle_x, circle_y, pos;
	int i=0;
	struct Portion portion;

	char *aux = malloc(sizeof(char) * width * height * 2);

	memset(aux, 0, width*height);

	/* Mark where the circle is */
	for (int y=top_y; y < bottom_y; ++y) {
		dy = center_y - y;
		for (int x=left_x; x < right_x; ++x) {
			dx = center_x - x;
			if (dy*dy + dx*dx < radius_sqr)
				c = 1;
			else
				c = 0;

			aux[y * width + x] = c;
		}
	}

	/* build compressed pie from markings */
	for (int y=top_y; y<bottom_y; y+=2) {
		for (int x=left_x; x<right_x; ++x) {
			pos = (int)((y+height/2)/2 * width + x + width/4);

			top = aux[y * width + x];
			bottom = aux[(y + 1) * width + x];
			canvas_screen[pos] = compression_table[bottom][top];

			/* Color portions */
			circle_x = x - center_x;
			circle_y = y - center_y;
			if (circle_x*circle_x + circle_y*circle_y <= scaled_radius*scaled_radius) {
				/* avoid NaNs */
				if ((divisor = circle_x + sqrt(circle_x*circle_x + circle_y*circle_y)) != 0) {
					radians = 2 * atan(circle_y/divisor);
					angle = RADIANS_TO_DEG(radians);
					angle = (angle < 0) ? CIRCLE_DEGREE_COUNT + angle : angle;
				} else {
					/* on the left side, when y = 0, use color from lines above
					 * to avoid angle 0 because of divisor being 0 */
					angle = (circle_x < 0) ? 170 : 0;
				}

				canvas_colors[pos] = pie->color_by_angle_map[(int)angle];
			}
		}

		/* print labels */
		if (i < pie->count_portions) {
			portion = pie->portion_stack[i];
			length = snprintf(NULL, 0, "%s %d%% ", portion.name, portion.percentage);
			pos = (i + height/2 - pie->count_portions/2) * width + labels_x_offset;

			snprintf(
					&canvas_screen[pos],
					length,
					"%s %d%% ",
					portion.name,
					portion.percentage
					);

			/* color it */
			for (int j=0; j < length; ++j)
				canvas_colors[pos + j] = portion.color;
			++i;
		}
	}
	free(aux);
	return 0;
}

Pie *
new_pie(double center_x, double center_y, double radius, double total)
{
	Pie *p = malloc(sizeof(Pie));
	p->center_x = center_x;
	p->center_y = center_y;
	p->radius = radius;
	p->total = total;
	p->count_portions = 0;
	p->color_map_color_count = 0;
 	/* TODO: use a variable for the blank color */
	for (int i=0; i<CIRCLE_DEGREE_COUNT; ++i )
		p->color_by_angle_map[i] = (Color)0;

	return p;
}

struct Portion *
new_portion(int percentage, char *name, Color color)
{
	struct Portion *p = malloc(sizeof(struct Portion));
	p->percentage = percentage;
	p->name = name;
	p->color = color;

	return p;
}

void
del_portion(struct Portion *p)
{
	free(p->name);
	free(p);
}

void
set_center_x(Pie *p, double center_x)
{
	p->center_x = center_x;
}

void
set_center_y(Pie *p, double center_y)
{
	p->center_y = center_y;
}

void
pie_push_portion(Pie *pie, struct Portion *portion)
{
	if (pie->count_portions >= MAX_PORTIONS)
		return;

	double portion_degree_count = (portion->percentage/100.0 * CIRCLE_DEGREE_COUNT);
	if (pie->color_map_color_count + portion_degree_count > CIRCLE_DEGREE_COUNT)
		return;

	/* mark in which angles the portion is */
	pie->portion_stack[pie->count_portions++] = *portion;
	for (int i=pie->color_map_color_count; i < (pie->color_map_color_count + portion_degree_count) && i < CIRCLE_DEGREE_COUNT; ++i)
		pie->color_by_angle_map[i] = portion->color;

	pie->color_map_color_count += portion_degree_count;
}

void
pie_pop_portion(Pie *pie)
{
	if (pie->count_portions <= 0)
		return;

	struct Portion portion = pie->portion_stack[pie->count_portions];

	int portion_degree_count = portion.percentage/100 * CIRCLE_DEGREE_COUNT;
	pie->color_map_color_count -= portion_degree_count;

	del_portion(&portion);
}

double
get_center_x(Pie *p)
{
	return p->center_x;
}

double
get_center_y(Pie *p)
{
	return p->center_y;
}

double
get_radius(Pie *p)
{
	return p->radius;
}

double
get_total(Pie *p)
{
	return p->total;
}
