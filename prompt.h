#ifndef _PROMPT_H
#define _PROMPT_H

/* GLOBAL VARIABLES */
extern WINDOW *prompt_win;
extern PANEL *prompt_panel;

extern char buffer[MAX_NAME_LENGTH];

extern int prompt_win_h;
extern int prompt_win_w;

/* FUNCTION PROTOTYPES */
void draw_headers(WINDOW *window, int height, int width, char *label, chtype color);
void print_align_center(WINDOW *win, int start_y, int start_x, int width, char *string, chtype color);
char *prompt_user(Canvas *c, char *question, char *label, int align_center);
void alert(Canvas *c, char *question, char *label, int align_center);

#endif
