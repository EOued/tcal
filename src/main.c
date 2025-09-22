#include "drawer.h"
#include "functions.h"
#include "macro.h"
#include "renderer.h"
#include "views_handling.h"
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
  initscr();
  noecho();
  curs_set(0);
  start_color();
  renderable* r = initRenderable();
  int box_uuid  = -1;
  renderableAdd(r, quit_text, NULL);

  MEMCREATE(int*, box_args, malloc(4 * sizeof(int)));
  MEMCREATE(int*, date_arg, malloc(3 * sizeof(int)));

  // Get current month and year
  time_t t          = time(NULL);
  struct tm* tminfo = localtime(&t);

  date_arg[0] = tminfo->tm_mday;
  date_arg[1] = tminfo->tm_mon;
  date_arg[2] = tminfo->tm_year + 1900;

  int view_uuid = renderableAdd(r, day_grid, date_arg);

  uint help_page = 0;

  views* v            = viewsInit();
  enum views view     = day;
  enum views old_view = none;
  for (enum views _v = help; _v <= month; _v++) createView(v, _v);

  MEMCREATE(HELP_ARG*, helpActionArg, calloc(1, sizeof(HELP_ARG)));
  helpActionArg->view     = &view;
  helpActionArg->old_view = &old_view;
  helpActionArg->r        = r;
  helpActionArg->uuid     = &box_uuid;
  helpActionArg->args     = &help_page;

  MEMCREATE(DAY_ARG*, dayActionArg, calloc(1, sizeof(DAY_ARG)));
  dayActionArg->view     = &view;
  dayActionArg->old_view = &old_view;
  dayActionArg->r        = r;
  dayActionArg->uuid     = &view_uuid;
  dayActionArg->args     = date_arg;

  MEMCREATE(WEEK_ARG*, weekActionArg, calloc(1, sizeof(WEEK_ARG)));
  weekActionArg->view     = &view;
  weekActionArg->old_view = &old_view;
  weekActionArg->r        = r;
  weekActionArg->uuid     = &view_uuid;
  weekActionArg->args     = date_arg;

  MEMCREATE(MONTH_ARG*, monthActionArg, calloc(1, sizeof(MONTH_ARG)));
  monthActionArg->view     = &view;
  monthActionArg->old_view = &old_view;
  monthActionArg->r        = r;
  monthActionArg->uuid     = &view_uuid;
  monthActionArg->args     = date_arg;

  for (enum views _v = day; _v <= month; _v++)
  {
    viewsAddAction(v, _v, '?', helpViewOpen, &helpActionArg);
    viewsAddAction(v, _v, 'q', quit, NULL);
  }

  viewsAddAction(v, help, '?', helpViewQuit, &helpActionArg);
  viewsAddAction(v, help, 'q', helpViewQuit, &helpActionArg);
  viewsAddAction(v, help, 'p', helpViewPreviousAction, &helpActionArg);
  viewsAddAction(v, help, 'n', helpViewNextAction, &helpActionArg);

  viewsAddAction(v, day, 'v', dayViewNext, &weekActionArg);
  viewsAddAction(v, day, 'n', dayNext, &dayActionArg);
  viewsAddAction(v, day, 'p', dayPrevious, &dayActionArg);
  // viewsAddAction(v, day, 't', dayToday, &dayActionArg);

  viewsAddAction(v, week, 'v', weekViewNext, &monthActionArg);
  viewsAddAction(v, week, 'j', weekCursorRight, &weekActionArg);
  viewsAddAction(v, week, 'l', weekCursorRight, &weekActionArg);
  viewsAddAction(v, week, 'k', weekCursorLeft, &weekActionArg);
  viewsAddAction(v, week, 'h', weekCursorLeft, &weekActionArg);
  viewsAddAction(v, week, 'n', weekNext, &weekActionArg);
  viewsAddAction(v, week, 'p', weekPrevious, &weekActionArg);

  viewsAddAction(v, month, 'v', monthViewNext, &dayActionArg);
  viewsAddAction(v, month, 'k', monthCursorUp, &monthActionArg);
  viewsAddAction(v, month, 'j', monthCursorDown, &monthActionArg);
  viewsAddAction(v, month, 'l', monthCursorRight, &monthActionArg);
  viewsAddAction(v, month, 'h', monthCursorLeft, &monthActionArg);
  viewsAddAction(v, month, 'n', monthNext, &monthActionArg);
  viewsAddAction(v, month, 'p', monthPrevious, &monthActionArg);

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
      int arg = viewsExecuteAction(v, view, ch);
      if (arg) goto leave;
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
  free(date_arg);
  free(helpActionArg);
  free(dayActionArg);
  free(weekActionArg);
  free(monthActionArg);
  return EXIT_SUCCESS;
}
