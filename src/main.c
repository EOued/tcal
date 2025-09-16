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
  void (*view_array[3])(void*) = {day_grid, week_grid, month_grid};
  unsigned int view_index      = 0;
  int view_uuid                = renderableAdd(r, view_array[view_index], NULL);
  int week_index               = 0;
  int month_index              = 0;
  RENDER(r);
  int to_render      = 0;
  unsigned int x_lim = COLS;
  unsigned int y_lim = LINES;
  while (1)
  {
    refresh();
    int ch = getch();
    switch (ch)
    {
    case 'q': endwin(); goto leave;
    case 'j':
      if (y < (int)y_lim) y++;
      to_render = 1;
      break;
    case 'k':
      if (y > 0) y--;
      to_render = 1;
      break;
    case 'l':
      if (x < (int)x_lim) x++;
      to_render = 1;
      break;
    case 'h':
      if (x > 0) x--;
      to_render = 1;
      break;
    case 'v':
      renderableRemove(r, view_uuid);
      view_index += 1;
      view_index %= 3;
      view_uuid = renderableAdd(r, view_array[view_index],
                                view_index == 2
                                    ? &month_index
                                    : (view_index == 1 ? &week_index : NULL));
      if (view_index == 2)
      {
        x_lim = 4;
        y_lim = 4;
      }
      to_render = 1;
      break;
    case '?':
      if (opened_popup) renderableRemove(r, box_uuid);
      else
        box_uuid = renderableAdd(r, _help_box, NULL);
      opened_popup = 1 - opened_popup;
      to_render    = 1;
      break;
    }
    if (view_index == 2)
    {
      month_index = x + y * 5;
      updateArgument(r, view_uuid, &month_index);
    }
    if (to_render)
    {
      RENDER(r);
      to_render = 0;
    }
  }
leave:
  freeRenderable(r);
  free(box_args);
  return EXIT_SUCCESS;
}
