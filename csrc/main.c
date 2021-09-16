/* INCLUDES */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "bar_chart.h"

/* GLOBAL VARIABLES */
static char chart[HEIGHT * WIDTH];

void
build_frame() {
	if (! FRAMED)
		return;
	char c1;
	char c2;

	for (int i=0; i < HEIGHT; ++i) {
		/* build border horizontally */
		if (i == HEIGHT-1 || i == 0) {
			c1 = FRAME_CORNER;
			c2 = FRAME_BORDER_H;
		}
		/* build border vertically */
		else {
			c1 = FRAME_BORDER_V;
			c2 = BLANK;
		}

		chart[i * WIDTH] = c1;
		chart[i * WIDTH + WIDTH - 1] = c1;
		for (int j=1; j < WIDTH-1; ++j)
			chart[i * WIDTH + j] = c2;
	}
}

void
show(char *title) {
	/* print title */
	printf("%s\n", title);
	for (int i=0; i < HEIGHT; ++i)
		printf("%.*s\n", WIDTH, &chart[i * WIDTH] );
}

void
init_chart()
{
	char *p = chart;
	int n = HEIGHT * WIDTH;

	while (n--) *p++ = BLANK;
}

int
main()
{
	int x_offset = 2;
	float scale = 50 / HEIGHT;
	Bar *b1 = new_bar(25, "lucca");
	Bar *b2 = new_bar(5, "luquinha");
	Bar *b3 = new_bar(50, "luccão");
	init_chart();
	build_frame();
	print_bar(b1, chart, x_offset, scale);
	x_offset += strlen(get_name(b1)) + 1;
	print_bar(b2, chart, x_offset, scale);
	x_offset += strlen(get_name(b2)) + 1;
	print_bar(b3, chart, x_offset, scale);

	show("TEST");
	return 0;
}
