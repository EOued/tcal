#include "drawer.h"
#include "macro.h"
#include "renderer.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

void quit_text(void* _)
{
  (void)_;
  move(LINES - 1, 0);
  addstr("Press q to exit, ? for help...");
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
  int opened_help = 0;
  renderable* r   = initRenderable();
  int box_uuid    = -1;
  int* box_args;
  MEMCHK((box_args = malloc(4 * sizeof(int))));
  renderableAdd(r, quit_text, NULL);
  void (*view_array[3])(void*) = {day_grid, week_grid, month_grid};
  unsigned int view_index      = 0;
  int view_uuid                = renderableAdd(r, view_array[view_index], NULL);
  int week_index               = 0;
  int* month_args;
  MEMCHK((month_args = calloc(3, sizeof(int))));
  // Get current month and year
  time_t t          = time(NULL);
  struct tm* tminfo = localtime(&t);
  month_args[1]     = tminfo->tm_mon;
  month_args[2]     = tminfo->tm_year + 1900;

  RENDER(r);
  int to_render          = 0;
  unsigned int help_page = 0;
  while (1)
  {
    refresh();
    int ch = getch();
    switch (ch)
    {
    case 'q':
      if (opened_help)
      {
        renderableRemove(r, box_uuid);
        opened_help = 0;
        RENDER_BREAK(to_render);
      }
      endwin();
      goto leave;
    case 'j':
      if (view_index == 1 && week_index < 4) week_index++;
      if (view_index == 2 && month_args[0] < 19) month_args[0] += 5;
      RENDER_BREAK(to_render);
    case 'k':
      if (view_index == 1 && week_index > 0) week_index--;
      if (view_index == 2 && month_args[0] > 4) month_args[0] -= 5;
      RENDER_BREAK(to_render);
    case 'l':
      if (view_index == 1 && week_index < 4) week_index++;
      if (view_index == 2 && month_args[0] < 24) month_args[0]++;
      RENDER_BREAK(to_render);
    case 'h':
      if (view_index == 1 && week_index > 0) week_index--;
      if (view_index == 2 && month_args[0] > 0) month_args[0]--;
      RENDER_BREAK(to_render);
    case 'v':
      view_index += 1;
      view_index %= 3;
      UPDATE_VIEW(view_index, r, view_uuid);
      if (view_index == 1) updateArgument(r, view_uuid, &week_index);
      if (view_index == 2) updateArgument(r, view_uuid, month_args);
      RENDER_BREAK(to_render);
    case ' ':
      view_index = 0;
      UPDATE_VIEW(view_index, r, view_uuid);
      RENDER_BREAK(to_render);
    case 'n':
      if (opened_help)
      {
        help_page++;
        updateArgument(r, box_uuid, &help_page);
        RENDER_BREAK(to_render);
      }
      if (view_index == 2)
      {
        month_args[1] = (month_args[1] + 1) % 12;
        if (!month_args[1]) month_args[2]++;
        RENDER_BREAK(to_render);
      }
      break;
    case 'p':
      if (opened_help && help_page > 0)
      {
        help_page--;
        updateArgument(r, box_uuid, &help_page);
        RENDER_BREAK(to_render);
      }
      if (view_index == 2)
      {
        month_args[1] = (month_args[1] - 1 + 12) % 12;
        if (month_args[1] == 11) month_args[2]--;
        if (month_args[2] < 1900)
        {
          month_args[2] = 1900;
          month_args[1] = 0;
        }
        RENDER_BREAK(to_render);
      }
      break;
    case KEY_RESIZE: RENDER_BREAK(to_render);
    case '?':
      help_page = 0;
      if (opened_help) renderableRemove(r, box_uuid);
      else
        box_uuid = renderableAdd(r, _help_box, &help_page);
      opened_help = 1 - opened_help;
      RENDER_BREAK(to_render);
    }
    if (view_index == 2) updateArgument(r, view_uuid, month_args);
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
  free(month_args);
  return EXIT_SUCCESS;
}
