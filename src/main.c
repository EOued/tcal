#include "calendar.h"
#include "drawer.h"
#include "functions.h"
#include "macro.h"
#include "renderer.h"
#include "views_handling.h"
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

// 293 lines

void quit_text(void* _)
{
  (void)_;
  move(LINES - 1, 0);
  addstr("Press q to exit, ? for help...");
  move(0, 0);
}

int main(void)
{
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(0);
  start_color();
  renderable* r = initRenderable();
  int box_uuid  = -1;
  renderableAdd(r, quit_text, NULL);

  MEMCREATE(int*, box_args, malloc(4 * sizeof(int)));
  MEMCREATE(view_arguments*, view_args, calloc(1, sizeof(view_arguments)));

  time_t now         = time(NULL);
  struct tm tm_today = *localtime(&now);
  // override for testing purposes
  // dec
  tm_today.tm_mon = 11;
  // eighth
  tm_today.tm_mday = 5;
  tm_today.tm_hour = 0;
  tm_today.tm_min  = 0;
  tm_today.tm_sec  = 0;

  view_args->date   = timegm(&tm_today);
  view_args->e_list = initCalendarTemplate();
  // Skips to next available day (useful if launched on a weekend)

  char buffer[100];
  strftime(buffer, sizeof(buffer), "%A %d %B %Y, %H:%M:%S", &tm_today);

  DAY_DECR(view_args->date);
  DAY_INCR(view_args->date);
  int view_uuid = renderableAdd(r, day_grid, view_args);

  uint help_page = 0;
  views* v       = viewsInit();

  enum views view     = day;
  enum views old_view = none;
  for (enum views _v = help; _v <= month; _v++) createView(v, _v);

  MEMCREATE(ARGS*, helpActionArg, calloc(1, sizeof(ARGS)));
  helpActionArg->view     = &view;
  helpActionArg->old_view = &old_view;
  helpActionArg->r        = r;
  helpActionArg->uuid     = &box_uuid;
  helpActionArg->args     = &help_page;

  MEMCREATE(ARGS*, dateActionArg, calloc(1, sizeof(ARGS)));
  dateActionArg->view     = &view;
  dateActionArg->old_view = &old_view;
  dateActionArg->r        = r;
  dateActionArg->uuid     = &view_uuid;
  dateActionArg->args     = view_args;

  // top-level
  viewsAddAction(v, -1, 'd', dayView, &dateActionArg);
  viewsAddAction(v, -1, 'w', weekView, &dateActionArg);
  viewsAddAction(v, -1, 'm', monthView, &dateActionArg);
  viewsAddAction(v, -1, '?', helpViewOpen, &helpActionArg);
  viewsAddAction(v, -1, 'q', quit, NULL);

  viewsAddAction(v, help, '?', helpViewQuit, &helpActionArg);
  viewsAddAction(v, help, 'q', helpViewQuit, &helpActionArg);
  viewsAddAction(v, help, 'p', helpViewPreviousAction, &helpActionArg);
  viewsAddAction(v, help, 'n', helpViewNextAction, &helpActionArg);

  viewsAddAction(v, day, 'n', dayNext, &dateActionArg);
  viewsAddAction(v, day, 'p', dayPrevious, &dateActionArg);

  viewsAddAction(v, week, 'k', dayNext, &dateActionArg);
  viewsAddAction(v, week, 'l', dayNext, &dateActionArg);
  viewsAddAction(v, week, 'j', dayPrevious, &dateActionArg);
  viewsAddAction(v, week, 'h', dayPrevious, &dateActionArg);
  viewsAddAction(v, week, 'n', weekNext, &dateActionArg);
  viewsAddAction(v, week, 'p', weekPrevious, &dateActionArg);

  viewsAddAction(v, month, 'j', weekNext, &dateActionArg);
  viewsAddAction(v, month, 'l', dayNext, &dateActionArg);
  viewsAddAction(v, month, 'k', weekPrevious, &dateActionArg);
  viewsAddAction(v, month, 'h', dayPrevious, &dateActionArg);
  viewsAddAction(v, month, 'n', monthNext, &dateActionArg);
  viewsAddAction(v, month, 'p', monthPrevious, &dateActionArg);
  RENDER(r);
  int to_render = 0;
  while (1)
  {
    refresh();
    int ch = getch();
    switch (ch)
    {
    case KEY_RESIZE: RENDER_BREAK(to_render);
    default:
    {
      // -1: Action not found in current level/toplevel
      // 0: Action found, executed successfully
      // 1: Action found, close window
      int arg = viewsExecuteAction(v, view, ch);
      if (arg > 0) goto leave;
      RENDER_BREAK(to_render);
    }
    }
    if (to_render)
    {
      RENDER(r);
      to_render = 0;
    }
  }
leave:
  freeRenderable(r);
  viewsFree(v);
  free(box_args);
  freeEventList(view_args->e_list);
  free(view_args);
  free(helpActionArg);
  free(dateActionArg);
  return EXIT_SUCCESS;
}
