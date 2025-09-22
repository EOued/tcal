#include "drawer.h"
#include "macro.h"
#include "views_handling.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int helpViewNextAction(void* varg)
{
  HELP_ARG** _arg = (HELP_ARG**)varg;
  HELP_ARG* arg   = *_arg;
  uint* ctrptr    = (uint*)arg->args;
  (*ctrptr)++;
  updateArgument(arg->r, *arg->uuid, ctrptr);
  return 0;
}

int helpViewPreviousAction(void* varg)
{
  HELP_ARG** _arg = (HELP_ARG**)varg;
  HELP_ARG* arg   = *_arg;
  uint* ctrptr    = (uint*)arg->args;

  (*ctrptr)--;
  updateArgument(arg->r, *arg->uuid, ctrptr);
  return 0;
}

int helpViewOpen(void* varg)
{
  if (varg == NULL) return 1;
  HELP_ARG** _arg = (HELP_ARG**)varg;
  HELP_ARG* arg   = *_arg;
  *arg->uuid      = renderableAdd(arg->r, _help_box, arg->args);
  *arg->old_view  = *arg->view;
  *arg->view      = help;
  return 0;
}

int helpViewQuit(void* varg)
{
  HELP_ARG** _arg = (HELP_ARG**)varg;
  HELP_ARG* arg   = *_arg;
  renderableRemove(arg->r, *arg->uuid);
  *arg->view     = *arg->old_view;
  *arg->old_view = help;
  return 0;
}

int dayView(void* varg)
{
  DATE_ARG** _arg = (DATE_ARG**)varg;
  DATE_ARG* arg   = *_arg;
  *arg->old_view  = *arg->view;
  *arg->view      = day;
  UPDATE_VIEW(arg->r, *arg->uuid, day_grid, arg->args);
  return 0;
}

int weekView(void* varg)
{
  DATE_ARG** _arg = (DATE_ARG**)varg;
  DATE_ARG* arg   = *_arg;
  *arg->old_view  = *arg->view;
  *arg->view      = week;
  UPDATE_VIEW(arg->r, *arg->uuid, week_grid, arg->args);
  return 0;
}

int monthView(void* varg)
{
  DATE_ARG** _arg = (DATE_ARG**)varg;
  DATE_ARG* arg   = *_arg;
  *arg->old_view  = *arg->view;
  *arg->view      = month;
  UPDATE_VIEW(arg->r, *arg->uuid, month_grid, arg->args);
  return 0;
}

int dayNext(void* varg)
{
  DATE_ARG** _arg = (DATE_ARG**)varg;
  DATE_ARG* arg   = *_arg;
  int* args       = (int*)arg->args;
  DAY_INCR(args);
  return 0;
}

int dayPrevious(void* varg)
{
  DATE_ARG** _arg = (DATE_ARG**)varg;
  DATE_ARG* arg   = *_arg;
  int* args       = (int*)arg->args;
  DAY_DECR(args);
  return 0;
}

int weekNext(void* varg)
{
  DATE_ARG** _arg = (DATE_ARG**)varg;
  DATE_ARG* arg   = *_arg;
  int* args       = (int*)arg->args;
  WEEK_INCR(args);
  return 0;
}

int weekPrevious(void* varg)
{
  DATE_ARG** _arg = (DATE_ARG**)varg;
  DATE_ARG* arg   = *_arg;
  int* args       = (int*)arg->args;
  WEEK_DECR(args);
  return 0;
}

int monthNext(void* varg)
{
  DATE_ARG** _arg = (DATE_ARG**)varg;
  DATE_ARG* arg   = *_arg;
  int* args       = (int*)arg->args;
  MONTH_INCR(args);
  return 0;
}

int monthPrevious(void* varg)
{
  DATE_ARG** _arg = (DATE_ARG**)varg;
  DATE_ARG* arg   = *_arg;
  int* args       = (int*)arg->args;
  MONTH_DECR(args);
  return 0;
}

int quit(void* _)
{
  (void)_;
  endwin();
  return 1;
}
