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

void
print_pie(Pie *pie, char *chart, float scale)
{
	//compute circle limits
	float center_x = get_center_x(pie);
	float center_y = get_center_y(pie);
	float scaled_radius = get_radius(pie) * scale;
	float left_x = center_x - scaled_radius;
	float right_x = center_x + scaled_radius;
	float top_y = center_y - scaled_radius;
	float bottom_y = center_y + scaled_radius;
	float radius_sqr = scaled_radius*scaled_radius;
	int c;
	float dx;
	float dy;

	for (int i=top_y; i < bottom_y; ++i) {
		dy = (center_y - i > 0) ? center_y - i : i - center_y;
		for (int j=left_x; j < right_x; ++j) {
			dx = (center_x - j > 0) ? center_x - j : j - center_x;
			if (dy*dy + dx*dx <= radius_sqr)
				c = PIEBLOCK_FULL;
			else
				c = BLANK;

			chart[i * WIDTH + j] = c;
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
