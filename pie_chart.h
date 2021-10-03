#ifndef PIE_CHART_H
#define PIE_CHART_H

#define PIE_OFFSET      1.5
#define PIEBLOCK_FULL   'O'
#define PIEBLOCK_TOP    '"'
#define PIEBLOCK_BOTTOM '_'

typedef struct PieChart Pie;

int print_pie(Pie *pie, int width, int height, char *canvas_screen, float scale);
Pie * new_pie (float center_x, float center_y, float radius, float total);
float get_center_x(Pie *p);
float get_center_y(Pie *p);
void set_center_x(Pie *p, float center_x);
void set_center_y(Pie *p, float center_y);
float get_radius(Pie *p);
float get_total(Pie *p);

#endif
