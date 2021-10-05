#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "pie_chart.h"
#include "utils.h"

struct Portion {
	int percentage;
	char *name;
	Color color;
};

struct PieChart {
	float center_x;
	float center_y;
	float radius;
	float total;
	struct Portion portion_stack[MAX_PORTIONS];
	int count_portions;
};

int
circle_fits(Pie *pie, int width, int height, char *canvas_screen, float scale)
{
	float center_x = get_center_x(pie);
	float center_y = get_center_y(pie);
	float scaled_radius = get_radius(pie) * scale;
	float scaled_radius_h = scaled_radius/2 + 0.5;

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
	int
	print_pie(Pie *pie, int width, int height, char *canvas_screen, float scale)
{
	int c, top, bottom, ret_code;
	// bottom and top are indices b  t
	static char compression_table[2][2] = {
	/*t         0                  1     */
	/*b0 */{BLANK,           PIEBLOCK_TOP },
	/*b1 */{PIEBLOCK_BOTTOM, PIEBLOCK_FULL},
	};

	/* check for screen boundaries */
	if ((ret_code = circle_fits(pie, width, height, canvas_screen, scale)) != PLOT_OK)
		return ret_code;

	/* compute circle limits */
	float dx;
	float dy;
	float center_x = get_center_x(pie);
	float center_y = get_center_y(pie);
	float scaled_radius = get_radius(pie) * scale;
	float left_x = center_x - scaled_radius;
	float right_x = center_x + scaled_radius;
	float top_y = center_y - scaled_radius;
	float bottom_y = center_y + scaled_radius;
	float radius_sqr = scaled_radius*scaled_radius;

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

	/* build compressed pie from canvas */
	for (int y=top_y; y<bottom_y; y+=2) {
		for (int x=left_x; x<right_x; ++x) {
			top = aux[y * width + x];
			bottom = aux[(y + 1) * width + x];
			canvas_screen[(int)((y+height/2)/2 * width + x + width/4)] = compression_table[bottom][top];
		}
	}

	free(aux);

	return 0;
}

Pie *
new_pie(float center_x, float center_y, float radius, float total)
{
	Pie *p = malloc(sizeof(Pie));
	p->center_x = center_x;
	p->center_y = center_y;
	p->radius = radius;
	p->total = total;
	p->count_portions = 0;

	return p;
}

void
set_center_x(Pie *p, float center_x)
{
	p->center_x = center_x;
}

void
set_center_y(Pie *p, float center_y)
{
	p->center_y = center_y;
}

void
pie_push_portion(Pie *pie, struct Portion *portion)
{
	if (pie->count_portions < MAX_PORTIONS)
		pie->portion_stack[pie->count_portions++] = *portion;
}

void
pie_pop_portion(Pie *pie)
{
	if (pie->count_portions > 0)
		pie->count_portions--;
}

float
get_center_x(Pie *p)
{
	return p->center_x;
}

float
get_center_y(Pie *p)
{
	return p->center_y;
}

float
get_radius(Pie *p)
{
	return p->radius;
}

float
get_total(Pie *p)
{
	return p->total;
}
