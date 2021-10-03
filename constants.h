#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FRAMED           0
#define PADDING          4
#define BLANK            ' '
#define MAX_NAME_LENGTH  50

/* COLOR CONSTANTS */
#define ANSI_COLOR_RED     "\x1b[31m"
#define	ANSI_COLOR_GREEN   "\x1b[32m"
#define	ANSI_COLOR_YELLOW  "\x1b[33m"
#define	ANSI_COLOR_BLUE    "\x1b[34m"
#define	ANSI_COLOR_MAGENTA "\x1b[35m"
#define	ANSI_COLOR_CYAN    "\x1b[36m"
#define	ANSI_COLOR_RESET   "\x1b[0m"

typedef char * ColorConstant;

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
