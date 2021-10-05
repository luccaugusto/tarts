#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FRAMED           0
#define PADDING          4
#define BLANK            ' '
#define MAX_NAME_LENGTH  50

typedef short Color;

typedef enum {
	FRAME_CORNER   = '+',
	FRAME_BORDER_H = '-',
	FRAME_BORDER_V = '|',
} Frame;

typedef enum {
	BACK = 0,
	FORE = 1,
} Pixel;

typedef enum {
	PLOT_OK = 0,
	ERR_CIRCLE_TOO_BIG,
	ERR_LINE_OUT,
} PlotStatus;

#endif
