#ifndef PIE_CHART_H
#define PIE_CHART_H

#define PIEBLOCK_FULL   'O'
#define PIEBLOCK_TOP    '"'
#define PIEBLOCK_BOTTOM '~'

typedef struct PieChart Pie;

void print_pie(Pie *pie, char *chart, float scale);
Pie * new_pie (float center_x, float center_y, float radius, float total);
float get_center_x(Pie *p);
float get_center_y(Pie *p);
float get_radius(Pie *p);
float get_total(Pie *p);

#endif
