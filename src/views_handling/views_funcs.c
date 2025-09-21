#include "drawer.h"
#include "macro.h"
#include "views_handling.h"
#include <ncurses.h>
#include <stdio.h>

static inline int do_skip_day(int day, MONTH m, int year)
{
  return week_day(day, m, year) > fri;
}

static inline int do_skip_week(int week_number, MONTH m, int year)
{
  int is_a_day_in_good_month = 0;
  int* md;
  for (int i = mon; i < sat; i++)
  {
    md = month_day(week_number, i, m, year);
    is_a_day_in_good_month |= (int)m == md[1];
    free(md);
  }
  return !is_a_day_in_good_month;
}

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
  *arg->uuid      = renderableAdd(arg->r, arg->_help_box, arg->args);
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
  do {
    CASC_DAY_INCR(dargs[0], dargs[1], dargs[2]);
  } while (do_skip_day(dargs[0], dargs[1], dargs[2]));
  return 0;
}

int dayPrevious(void* varg)
{
  DAY_ARG** _arg = (DAY_ARG**)varg;
  DAY_ARG* arg   = *_arg;
  int* dargs     = (int*)arg->args;
  do {
    CASC_DAY_DECR(dargs[0], dargs[1], dargs[2]);
    WDMONTH_LIMIT(dargs[0], dargs[1], dargs[2]);
  } while (do_skip_day(dargs[0], dargs[1], dargs[2]));
  return 0;
}

int weekCursorRight(void* varg)
{
  WEEK_ARG** _arg = (WEEK_ARG**)varg;
  WEEK_ARG* arg   = *_arg;
  int* wargs      = (int*)arg->args;
  if (wargs[0] < 4) wargs[0]++;
  return 0;
}

int weekCursorLeft(void* varg)
{
  WEEK_ARG** _arg = (WEEK_ARG**)varg;
  WEEK_ARG* arg   = *_arg;
  int* wargs      = (int*)arg->args;
  if (wargs[0] > 0) wargs[0]--;
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
  do {
    CASC_WEEK_INCR(wargs[1], wargs[2], wargs[3]);
  } while (do_skip_week(wargs[1], wargs[2], wargs[3]));
  return 0;
}

int weekPrevious(void* varg)
{
  WEEK_ARG** _arg = (WEEK_ARG**)varg;
  WEEK_ARG* arg   = *_arg;
  int* wargs      = (int*)arg->args;
  do {
    CASC_WEEK_DECR(wargs[1], wargs[2], wargs[3]);
    WDMONTH_LIMIT(wargs[1], wargs[2], wargs[3])
  } while (do_skip_week(wargs[1], wargs[2], wargs[3]));
  return 0;
}

int monthCursorUp(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  if (margs[0] > 4) margs[0] -= 5;
  return 0;
}
int monthCursorDown(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  if (margs[0] < 19) margs[0] += 5;
  return 0;
}

int monthCursorRight(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  if (margs[0] < 24) margs[0]++;
  return 0;
}

int monthCursorLeft(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  if (margs[0] > 0) margs[0]--;
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
  CASC_MONTH_INCR(margs[1], margs[2]);
  return 0;
}

int monthPrevious(void* varg)
{
  MONTH_ARG** _arg = (MONTH_ARG**)varg;
  MONTH_ARG* arg   = *_arg;
  int* margs       = (int*)arg->args;
  CASC_MONTH_DECR(margs[1], margs[2]);
  MONTH_LIMIT(margs[1], margs[2]);
  return 0;
}

int quit(void* _)
{
  (void)_;
  endwin();
  return 1;
}
