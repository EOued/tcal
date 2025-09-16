#include "drawer.h"
#include "renderer.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

void quit_text(void* _)
{
  (void)_;
  move(LINES - 1, 0);
  addstr("Press q to exit, h for help...");
  move(0, 0);
}

void redraw(void)
{
  // Add all rendered functions to a list. Upon redraw, we clear and call all of
  // that functions.
  return;
}

int main(void)
{
  initscr();
  noecho();
  curs_set(0);
  start_color();
  int x            = 0;
  int y            = 0;
  int opened_popup = 0;
  renderable* r    = initRenderable();
  int box_uuid     = -1;
  int* box_args    = calloc(4, sizeof(int));
  renderableAdd(r, quit_text, NULL);
  void (*view_array[2])(void*) = {week_grid, month_grid};
  unsigned int view_index      = 0;
  int view_uuid                = renderableAdd(r, view_array[view_index], NULL);
  RENDER(r);
  while (1)
  {
    refresh();
    int ch = getch();
    switch (ch)
    {
    case 'q': endwin(); goto leave;
    case 'j':
      if (y < LINES - 1) y++;
      break;
    case 'k':
      if (y > 0) y--;
      break;
    case 'l':
      if (x < COLS - 1) x++;
      break;
    case 'h':
      if (x > 0) x--;
      break;
    case 'v':
      renderableRemove(r, view_uuid);
      view_index = 1 - view_index;
      view_uuid  = renderableAdd(r, view_array[view_index], NULL);
      break;
    case '?':
      if (opened_popup) renderableRemove(r, box_uuid);
      else
        box_uuid = renderableAdd(r, _help_box, NULL);
      opened_popup = 1 - opened_popup;
      break;
    }
    RENDER(r);
    move(y, x);
  }
leave:
  freeRenderable(r);
  free(box_args);
  return EXIT_SUCCESS;
}
