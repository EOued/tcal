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

  MEMCREATE(DATE_ARG*, dateActionArg, calloc(1, sizeof(DATE_ARG)));
  dateActionArg->view     = &view;
  dateActionArg->old_view = &old_view;
  dateActionArg->r        = r;
  dateActionArg->uuid     = &view_uuid;
  dateActionArg->args     = date_arg;

  for (enum views _v = day; _v <= month; _v++)
  {
    viewsAddAction(v, _v, '?', helpViewOpen, &helpActionArg);
    viewsAddAction(v, _v, 'q', quit, NULL);
  }

  viewsAddAction(v, help, '?', helpViewQuit, &helpActionArg);
  viewsAddAction(v, help, 'q', helpViewQuit, &helpActionArg);
  viewsAddAction(v, help, 'p', helpViewPreviousAction, &helpActionArg);
  viewsAddAction(v, help, 'n', helpViewNextAction, &helpActionArg);

  viewsAddAction(v, day, 'v', dayViewNext, &dateActionArg);
  viewsAddAction(v, day, 'n', dayNext, &dateActionArg);
  viewsAddAction(v, day, 'p', dayPrevious, &dateActionArg);
  // viewsAddAction(v, day, 't', dayToday, &dayActionArg);

  viewsAddAction(v, week, 'v', weekViewNext, &dateActionArg);
  viewsAddAction(v, week, 'j', dayNext, &dateActionArg);
  viewsAddAction(v, week, 'l', dayNext, &dateActionArg);
  viewsAddAction(v, week, 'k', dayPrevious, &dateActionArg);
  viewsAddAction(v, week, 'h', dayPrevious, &dateActionArg);
  viewsAddAction(v, week, 'n', weekNext, &dateActionArg);
  viewsAddAction(v, week, 'p', weekPrevious, &dateActionArg);

  viewsAddAction(v, month, 'v', monthViewNext, &dateActionArg);
  viewsAddAction(v, month, 'k', weekPrevious, &dateActionArg);
  viewsAddAction(v, month, 'j', weekNext, &dateActionArg);
  viewsAddAction(v, month, 'l', dayNext, &dateActionArg);
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
  free(dateActionArg);
  return EXIT_SUCCESS;
}
