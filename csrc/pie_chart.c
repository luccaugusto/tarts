#include <string.h>
#include <stdlib.h>

#include "constants.h"
#include "pie_chart.h"
#include "utils.h"

struct PieChart {
	float center_x;
	float center_y;
	float radius;
	float total;
};

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
void
print_pie(Pie *pie, char *chart, float scale)
{
	int c, top, bottom;
	// bottom and top are indices b  t
	static char compression_table[2][2] = {
	/*          0                  1     */
	/* 0 */{BLANK,           PIEBLOCK_TOP },
	/* 1 */{PIEBLOCK_BOTTOM, PIEBLOCK_FULL},
	};

	//compute circle limits
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

	char *compressed_pie = malloc(sizeof(char) * (bottom_y - top_y) * (right_x - left_x));

	for (int y=top_y; y < bottom_y; ++y) {
		dy = (center_y - y > 0) ? center_y - y : y - center_y;
		for (int x=left_x; x < right_x; ++x) {
			dx = (center_x - x > 0) ? center_x - x : x - center_x;
			if (dy*dy + dx*dx <= radius_sqr)
				c = 1;
			else
				c = 0;

			chart[y * WIDTH + x] = c;
		}
	}

	/* build compressed pie from chart */
	for (int y=top_y; y<bottom_y; y+=2) {
		for (int x=left_x; x<right_x; ++x) {
			top = chart[y * WIDTH + x];
			bottom = chart[(y + 1) * WIDTH + x];
			compressed_pie[(y-(int)top_y)*WIDTH + (x-(int)left_x)] = compression_table[bottom][top];
		}
	}

	/* write compressed pie to chart */
	for (int y=top_y; y<bottom_y; ++y) {
		for (int x=left_x; x<right_x; ++x) {
			chart[(y/2 + (int)(top_y * 1.5))*WIDTH + x] = compressed_pie[(y-(int)top_y)*WIDTH + (x-(int)left_x)];
		}
	}
}

Pie *
new_pie (float center_x, float center_y, float radius, float total)
{
	Pie *p = malloc(sizeof(Pie));
	p->center_x = center_x;
	p->center_y = center_y;
	p->radius = radius;
	p->total = total;

	return p;
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
