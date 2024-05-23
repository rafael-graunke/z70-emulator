#ifndef SCREEN_H_
#define SCREEN_H_

#include <ncurses.h>
#include <stdbool.h>

typedef struct s_screen
{
    WINDOW *main;
    WINDOW *memory;
    WINDOW *instructions;
    WINDOW *registers;
    WINDOW *input;
    int x, y;
} Screen;

Screen *create_screen(void);
void destroy_screen(Screen *screen);
void print_alert(Screen *screen, char *message);

#endif