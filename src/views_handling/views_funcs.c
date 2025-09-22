#include "drawer.h"
#include "macro.h"
#include "views_handling.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int helpViewNextAction(void* varg)
{
  ARGS** _arg  = (ARGS**)varg;
  ARGS* arg    = *_arg;
  uint* ctrptr = (uint*)arg->args;
  (*ctrptr)++;
  updateArgument(arg->r, *arg->uuid, ctrptr);
  return 0;
}

int helpViewPreviousAction(void* varg)
{
  ARGS** _arg  = (ARGS**)varg;
  ARGS* arg    = *_arg;
  uint* ctrptr = (uint*)arg->args;

  (*ctrptr)--;
  updateArgument(arg->r, *arg->uuid, ctrptr);
  return 0;
}

int helpViewOpen(void* varg)
{
  if (varg == NULL) return 1;
  ARGS** _arg    = (ARGS**)varg;
  ARGS* arg      = *_arg;
  *arg->uuid     = renderableAdd(arg->r, _help_box, arg->args);
  *arg->old_view = *arg->view;
  *arg->view     = help;
  return 0;
}

int helpViewQuit(void* varg)
{
  ARGS** _arg = (ARGS**)varg;
  ARGS* arg   = *_arg;
  renderableRemove(arg->r, *arg->uuid);
  *arg->view     = *arg->old_view;
  *arg->old_view = help;
  return 0;
}

int dayView(void* varg)
{
  ARGS** _arg    = (ARGS**)varg;
  ARGS* arg      = *_arg;
  *arg->old_view = *arg->view;
  *arg->view     = day;
  UPDATE_VIEW(arg->r, *arg->uuid, day_grid, arg->args);
  return 0;
}

int weekView(void* varg)
{
  ARGS** _arg    = (ARGS**)varg;
  ARGS* arg      = *_arg;
  *arg->old_view = *arg->view;
  *arg->view     = week;
  UPDATE_VIEW(arg->r, *arg->uuid, week_grid, arg->args);
  return 0;
}

int monthView(void* varg)
{
  ARGS** _arg    = (ARGS**)varg;
  ARGS* arg      = *_arg;
  *arg->old_view = *arg->view;
  *arg->view     = month;
  UPDATE_VIEW(arg->r, *arg->uuid, month_grid, arg->args);
  return 0;
}

int dayNext(void* varg)
{
  ARGS** _arg = (ARGS**)varg;
  ARGS* arg   = *_arg;
  int* args   = (int*)arg->args;
  DAY_INCR(args);
  return 0;
}

int dayPrevious(void* varg)
{
  ARGS** _arg = (ARGS**)varg;
  ARGS* arg   = *_arg;
  int* args   = (int*)arg->args;
  DAY_DECR(args);
  return 0;
}

int weekNext(void* varg)
{
  ARGS** _arg = (ARGS**)varg;
  ARGS* arg   = *_arg;
  int* args   = (int*)arg->args;
  WEEK_INCR(args);
  return 0;
}

int weekPrevious(void* varg)
{
  ARGS** _arg = (ARGS**)varg;
  ARGS* arg   = *_arg;
  int* args   = (int*)arg->args;
  WEEK_DECR(args);
  return 0;
}

int monthNext(void* varg)
{
  ARGS** _arg = (ARGS**)varg;
  ARGS* arg   = *_arg;
  int* args   = (int*)arg->args;
  MONTH_INCR(args);
  return 0;
}

int monthPrevious(void* varg)
{
  ARGS** _arg = (ARGS**)varg;
  ARGS* arg   = *_arg;
  int* args   = (int*)arg->args;
  MONTH_DECR(args);
  return 0;
}

int quit(void* _)
{
  (void)_;
  endwin();
  return 1;
}
