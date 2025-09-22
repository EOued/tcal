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

int dayViewNext(void* varg)
{
  WEEK_ARG** _arg = (WEEK_ARG**)varg;
  WEEK_ARG* arg   = *_arg;
  *arg->old_view  = day;
  *arg->view      = week;
  UPDATE_VIEW(arg->r, *arg->uuid, week_grid, arg->args);
  return 0;
}

int dayNext(void* varg)
{
  DAY_ARG** _arg = (DAY_ARG**)varg;
  DAY_ARG* arg   = *_arg;
  int* dargs     = (int*)arg->args;
  DAY_INCR(dargs);
  return 0;
}

int dayPrevious(void* varg)
{
  DAY_ARG** _arg = (DAY_ARG**)varg;
  DAY_ARG* arg   = *_arg;
  int* dargs     = (int*)arg->args;
  DAY_DECR(dargs);
  return 0;
}

int weekCursorRight(void* varg)
{
  WEEK_ARG** _arg = (WEEK_ARG**)varg;
  WEEK_ARG* arg   = *_arg;
  int* wargs      = (int*)arg->args;
  DAY_INCR(wargs);
  return 0;
}

int weekCursorLeft(void* varg)
{
  WEEK_ARG** _arg = (WEEK_ARG**)varg;
  WEEK_ARG* arg   = *_arg;
  int* wargs      = (int*)arg->args;
  DAY_DECR(wargs);
  return 0;
}

int weekViewNext(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  *arg->old_view   = month;
  *arg->view       = month;
  UPDATE_VIEW(arg->r, *arg->uuid, month_grid, arg->args);
  return 0;
}

int weekNext(void* varg)
{
  WEEK_ARG** _arg = (WEEK_ARG**)varg;
  WEEK_ARG* arg   = *_arg;
  int* wargs      = (int*)arg->args;
  WEEK_INCR(wargs);
  return 0;
}

int weekPrevious(void* varg)
{
  WEEK_ARG** _arg = (WEEK_ARG**)varg;
  WEEK_ARG* arg   = *_arg;
  int* wargs      = (int*)arg->args;
  WEEK_DECR(wargs);
  return 0;
}

int monthCursorUp(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  WEEK_DECR(margs);
  return 0;
}
int monthCursorDown(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  WEEK_INCR(margs);
  return 0;
}

int monthCursorRight(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  DAY_INCR(margs);
  return 0;
}

int monthCursorLeft(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  DAY_DECR(margs);
  return 0;
}

int monthViewNext(void* varg)
{
  DAY_ARG** _arg = (DAY_ARG**)varg;
  DAY_ARG* arg   = *_arg;
  *arg->old_view = day;
  *arg->view     = day;
  UPDATE_VIEW(arg->r, *arg->uuid, day_grid, arg->args);
  return 0;
}

int monthNext(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  MONTH_INCR(margs);
  return 0;
}

int monthPrevious(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  MONTH_DECR(margs);
  return 0;
}

int quit(void* _)
{
  (void)_;
  endwin();
  return 1;
}
