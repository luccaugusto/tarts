/* INCLUDES */
#include <stdlib.h>
#include <stdio.h>

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

int
main()
{
	Bar *b1 = new_bar(25, "lucca");
	build_frame();
	print_bar(b1, chart, 0);
	show(get_name(b1));
	return 0;
}
