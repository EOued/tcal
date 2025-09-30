#include "drawer.h"
#include "macro.h"
#include "views_handling.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int helpViewNextAction(void* varg)
{
  VIEW_CHECKS(varg);
  uint* ctrptr = (uint*)arg->args;
  (*ctrptr)++;
  updateArgument(arg->r, *arg->uuid, ctrptr);
  return 0;
}

int helpViewPreviousAction(void* varg)
{
  VIEW_CHECKS(varg);
  uint* ctrptr = (uint*)arg->args;
  (*ctrptr)--;
  updateArgument(arg->r, *arg->uuid, ctrptr);
  return 0;
}

int helpViewOpen(void* varg)
{
  VIEW_CHECKS(varg);
  if (varg == NULL) return 1;
  *arg->uuid     = renderableAdd(arg->r, _help_box, arg->args);
  *arg->old_view = *arg->view;
  *arg->view     = help;
  return 0;
}

int helpViewQuit(void* varg)
{
  VIEW_CHECKS(varg);
  renderableRemove(arg->r, *arg->uuid);
  *arg->view     = *arg->old_view;
  *arg->old_view = help;
  return 0;
}

int dayView(void* varg)
{
  VIEW_CHECKS(varg);
  *arg->old_view = *arg->view;
  *arg->view     = day;
  UPDATE_VIEW(arg->r, *arg->uuid, day_grid, arg->args);
  return 0;
}

int weekView(void* varg)
{
  VIEW_CHECKS(varg);
  *arg->old_view = *arg->view;
  *arg->view     = week;
  UPDATE_VIEW(arg->r, *arg->uuid, week_grid, arg->args);
  return 0;
}

int monthView(void* varg)
{
  VIEW_CHECKS(varg);
  *arg->old_view = *arg->view;
  *arg->view     = month;
  UPDATE_VIEW(arg->r, *arg->uuid, month_grid, arg->args);
  return 0;
}

int dayNext(void* varg)
{
  VIEW_CHECKS(varg);
  view_arguments* v_arguments = (view_arguments*)arg->args;
  int* date                   = v_arguments->date;
  DAY_INCR(date);
  return 0;
}

int dayPrevious(void* varg)
{
  VIEW_CHECKS(varg);
  view_arguments* v_arguments = (view_arguments*)arg->args;
  int* args                   = v_arguments->date;
  DAY_DECR(args);
  return 0;
}

int weekNext(void* varg)
{
  VIEW_CHECKS(varg);
  view_arguments* v_arguments = (view_arguments*)arg->args;
  int* args                   = v_arguments->date;
  WEEK_INCR(args);
  return 0;
}

int weekPrevious(void* varg)
{
  VIEW_CHECKS(varg);
  view_arguments* v_arguments = (view_arguments*)arg->args;
  int* args                   = v_arguments->date;
  WEEK_DECR(args);
  return 0;
}

int monthNext(void* varg)
{
  VIEW_CHECKS(varg);
  view_arguments* v_arguments = (view_arguments*)arg->args;
  int* args                   = v_arguments->date;
  MONTH_INCR(args);
  return 0;
}

int monthPrevious(void* varg)
{
  VIEW_CHECKS(varg);
  view_arguments* v_arguments = (view_arguments*)arg->args;
  int* args                   = v_arguments->date;
  MONTH_DECR(args);
  return 0;
}

int quit(void* _)
{
  (void)_;
  endwin();
  return 1;
}
