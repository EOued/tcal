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
  int to_render = 0;
  while (1)
  {
    refresh();
    int ch = getch();
    switch (ch)
    {
    case 'q': endwin(); goto leave;
    case 'j':
      if (view_index == 1 && week_index < 4) week_index++;
      if (view_index == 2 && month_index < 19) month_index += 5;
      RENDER_BREAK(to_render);
    case 'k':
      if (view_index == 1 && week_index > 0) week_index--;
      if (view_index == 2 && month_index > 0) month_index -= 5;
      RENDER_BREAK(to_render);
    case 'l':
      if (view_index == 1 && week_index < 4) week_index++;
      if (view_index == 2 && month_index < 24) month_index++;
      RENDER_BREAK(to_render);
    case 'h':
      if (view_index == 1 && week_index > 0) week_index--;
      if (view_index == 2 && month_index > 0) month_index--;
      RENDER_BREAK(to_render);
    case 'v':
      view_index += 1;
      view_index %= 3;
      UPDATE_VIEW(view_index, r, view_uuid);
      if (view_index == 1) updateArgument(r, view_uuid, &week_index);
      if (view_index == 2) updateArgument(r, view_uuid, &month_index);
      RENDER_BREAK(to_render);
    case ' ':
      view_index = 0;
      UPDATE_VIEW(view_index, r, view_uuid);
      RENDER_BREAK(to_render);
    case '?':
      if (opened_popup) renderableRemove(r, box_uuid);
      else
        box_uuid = renderableAdd(r, _help_box, NULL);
      opened_popup = 1 - opened_popup;

      RENDER_BREAK(to_render);
    }
    if (view_index == 2) updateArgument(r, view_uuid, &month_index);
    if (view_index == 1) updateArgument(r, view_uuid, &week_index);

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
