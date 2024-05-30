#include <stdlib.h>
#include "screen.h"

Screen *create_screen(void)
{
    Screen *screen;
    if ((screen = malloc(sizeof(Screen))) == NULL)
        return NULL;

    screen->main = initscr();
    start_color();
    use_default_colors();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    refresh();

    getmaxyx(screen->main, screen->y, screen->x);
    screen->memory = newwin(screen->y - 6, screen->x * 2 / 3, 0, 0);
    screen->instructions = newwin(screen->y, screen->x / 3, 0, screen->x * 2 / 3);
    screen->registers = newwin(3, screen->x * 2 / 3, screen->y - 6, 0);
    screen->input = newwin(3, screen->x * 2 / 3, screen->y - 3, 0);

    return screen;
}

void destroy_screen(Screen *screen)
{
    endwin();
    free(screen);
}

void print_alert(Screen *screen, char *message)
{
    WINDOW *alert = newwin(7, 20, (screen->y / 2) - 3, (screen->x / 2) - 10);
    box(alert, 0, 0);
    wrefresh(alert);
    mvwprintw(alert, 3, 6, "FINISHED");
    wgetch(alert);
    wclear(alert);
    delwin(alert);
    refresh();
}
