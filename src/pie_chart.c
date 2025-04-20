#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "./utils.h"
#include "./constants.h"
#include "./pie_chart.h"

struct Portion {
	int percentage;
	double value;
	char *name;
	Color color;
};

struct PieChart {
	double center_x;
	double center_y;
	double radius;
	struct Portion portion_stack[MAX_PORTIONS];
	int count_portions;
	Color color_by_angle_map[CIRCLE_DEGREE_COUNT];
	int color_map_color_count;
};

int
circle_fits(Pie *pie, int width, int height)
{
	double center_x = get_center_x(pie);
	double center_y = get_center_y(pie);
	double radius = get_radius(pie);
	double radius_h = radius/2;

	if (2 * radius >= width || 2 * radius_h >= height)
		return ERR_CIRCLE_TOO_BIG;

	if (center_x + radius >= width) {
		center_x = (center_x + radius - width);

		if (center_x - radius <= 0)
			return ERR_CIRCLE_TOO_BIG;

		set_center_x(pie, center_x);
	}

	if (center_x - radius <= 0) {
		center_x = center_x + -(center_x - radius);

		if (center_x + radius >= width)
			return ERR_CIRCLE_TOO_BIG;

		set_center_x(pie, center_x);
	}

	/* y gets compressed so we can test with radius/2 */
	if (center_y + radius_h >= height) {
		center_y = (center_y + radius_h - height);

		if (center_y - radius_h <= 0)
			return ERR_CIRCLE_TOO_BIG;

		set_center_y(pie, center_y);
	}

	if (center_y - radius_h <= 0) {
		center_y = center_y - (center_y - radius_h);

		if (center_y + radius_h + 1 >= height)
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
 * ROW 3 [ ,",~,O]
 * these characters can be changed in the macros
 * BLANK, PIEBLOCK_TOP, PIEBLOCK_BOTTOM and PIEBLOCK_FULL respectively
 *  */
PlotStatus
print_pie(void *p, struct Dimentions *dimentions, char *canvas_screen, Color *canvas_colors)
{
	struct PieChart *pie = (struct PieChart *)p;
	int c, top, bottom, ret_code, length;
	int height = dimentions->height;
	int width = dimentions->width;
	/* check for screen boundaries */
	if ((ret_code = circle_fits(pie, width, height)) != PLOT_OK)
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
	double radius = get_radius(pie);
	double left_x = center_x - radius;
	double right_x = center_x + radius;
	double top_y = center_y - radius;
	double bottom_y = center_y + radius;
	double radius_sqr = radius*radius;
	int labels_x_offset = PADDING / 2;
	int circle_x, circle_y, pos;
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
			if (circle_x*circle_x + circle_y*circle_y < radius*radius) {
				/* avoid NaNs */
				if ((divisor = circle_x + sqrt(circle_x*circle_x + circle_y*circle_y)) != 0) {
					radians = 2 * atan(circle_y/divisor);
					angle = RADIANS_TO_DEG(radians);
					angle = (angle < 0) ? CIRCLE_DEGREE_COUNT + angle : angle;
				} else {
					/* on the left side, when y = 0, use color from lines above
					 * to avoid angle 0 because of divisor being 0 */
					angle = (circle_x <= 0) ? 170 : 0;
				}

				canvas_colors[pos] = pie->color_by_angle_map[(int)angle];
			}
		}
	}

	/* print labels */
	aux = malloc(sizeof(char) * MAX_NAME_LENGTH * 2);
	pos = (height-2) * width + labels_x_offset;
	for (int i=0; i < pie->count_portions; ++i) {
		portion = pie->portion_stack[i];

		snprintf(aux, MAX_NAME_LENGTH * 2, "%s %.2f(%d%%)", portion.name, portion.value, portion.percentage);
		length = strlen(aux);
		strncpy(&canvas_screen[pos], aux, length);

		/* color it */
		for (int j=0; j < length; ++j)
			canvas_colors[pos + j] = portion.color;
		pos += length + 1;
	}
	free(aux);
	return 0;
}

Pie *
new_pie(double center_y, double center_x)
{
	Pie *p = malloc(sizeof(Pie));
	p->center_x = center_x;
	p->center_y = center_y;
	p->radius = MAX_RADIUS;
	p->count_portions = 0;
	p->color_map_color_count = 0;
	for (int i=0; i<CIRCLE_DEGREE_COUNT; ++i )
		p->color_by_angle_map[i] = COLOR_BLANK;

	return p;
}

struct Portion *
new_portion(int percentage, double value, char *name, Color color)
{
	struct Portion *p = malloc(sizeof(struct Portion));
	p->percentage = percentage;
	p->name = name;
	p->value = value;
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

	double portion_degree_count = (portion->percentage/100.0 * (double)CIRCLE_DEGREE_COUNT);
	if (pie->color_map_color_count + portion_degree_count > CIRCLE_DEGREE_COUNT)
		return;

	pie->portion_stack[pie->count_portions++] = *portion;

	/* mark in which angles the portion is */
	int end_angle = pie->color_map_color_count + portion_degree_count;
	for (int i=pie->color_map_color_count; i < end_angle; ++i)
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

Portion *
get_portion_by_index(Pie *p, int index)
{
	if (index < 0 || index >= p->count_portions)
		return NULL;
	return &p->portion_stack[index];
}

int
get_portion_count(Pie *p)
{
	return p->count_portions;
}

int
portion_get_value(Portion *p)
{
	return p->value;
}

int pie_get_stats(Pie *p, double *avg, double *min, double *max, double *total, char *min_label, char *max_label)
{
	*total = 0;
	*min = *max = p->portion_stack[0].value;
	strncpy(min_label, p->portion_stack[0].name, MAX_NAME_LENGTH);
	for (int j=0; j<p->count_portions; ++j) {
		*total+=p->portion_stack[j].value;
		if (p->portion_stack[j].value < *min) {
			*min = p->portion_stack[j].value;
			strncpy(min_label, p->portion_stack[j].name, MAX_NAME_LENGTH);
		}

		if (p->portion_stack[j].value > *max) {
			*max = p->portion_stack[j].value;
			strncpy(max_label, p->portion_stack[j].name, MAX_NAME_LENGTH);
		}
	}
	*avg = *total/p->count_portions;
	return 1;
}
