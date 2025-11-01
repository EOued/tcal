#include "drawer.h"
#include "list.h"
#include "macro.h"
#include "views_handling.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int helpViewNextAction(ARGS** args)
{
  if (!args) return 1;
  uint* ctrptr = (*args)->args;
  (*ctrptr)++;
  updateArgument((*args)->r, *(*args)->uuid, ctrptr);
  return 0;
}

int helpViewPreviousAction(ARGS** args)
{
  if (!args) return 1;
  uint* ctrptr = (*args)->args;
  (*ctrptr)--;
  updateArgument((*args)->r, *(*args)->uuid, ctrptr);
  return 0;
}

int drawEventsOpen(ARGS** args)
{
  if (!args) return 1;
  *(*args)->uuid = renderableAdd((*args)->r, draw_event, (*args)->args);
  enum views v   = event_view;
  insertElement((*args)->view, &v);
  return 0;
}

int drawEventsQuit(ARGS** args)
{
  if (!args) return 1;
  renderableRemove((*args)->r, *(*args)->uuid);
  pop((*args)->view);
  return 0;
}

int helpViewQuit(ARGS** args)
{
  if (!args) return 1;
  renderableRemove((*args)->r, *(*args)->uuid);
  pop((*args)->view);
  return 0;
}

int dayView(ARGS** args)
{
  if (!args) return 1;
  enum views v = day;
  insertElement((*args)->view, &v);
  UPDATE_VIEW((*args)->r, *(*args)->uuid, day_grid, (*args)->args);
  return 0;
}

int weekView(ARGS** args)
{
  if (!args) return 1;
  enum views v = week;
  insertElement((*args)->view, &v);
  UPDATE_VIEW((*args)->r, *(*args)->uuid, week_grid, (*args)->args);
  return 0;
}

int monthView(ARGS** args)
{
  if (!args) return 1;
  enum views v = month;
  insertElement((*args)->view, &v);
  UPDATE_VIEW((*args)->r, *(*args)->uuid, month_grid, (*args)->args);
  return 0;
}

int dayNext(ARGS** args)
{
  if (!args) return 1;
  view_arguments* v_arguments = (*args)->args;
  DAY_INCR(v_arguments->date);
  return 0;
}

int dayPrevious(ARGS** args)
{
  if (!args) return 1;
  view_arguments* v_arguments = (*args)->args;
  DAY_DECR(v_arguments->date);
  return 0;
}

int weekNext(ARGS** args)
{
  if (!args) return 1;
  view_arguments* v_arguments = (*args)->args;
  WEEK_INCR(v_arguments->date);
  return 0;
}

int weekPrevious(ARGS** args)
{
  if (!args) return 1;
  view_arguments* v_arguments = (*args)->args;
  WEEK_DECR(v_arguments->date);
  return 0;
}

int monthNext(ARGS** args)
{
  if (!args) return 1;
  view_arguments* v_arguments = (*args)->args;
  MONTH_INCR(v_arguments->date);
  return 0;
}

int monthPrevious(ARGS** args)
{
  if (!args) return 1;
  view_arguments* v_arguments = (*args)->args;
  MONTH_DECR(v_arguments->date);
  return 0;
}

int currentDate(ARGS** args)
{
  time_t now                  = time(NULL);
  view_arguments* v_arguments = (*args)->args;
  struct tm tm_today          = *localtime(&now);
  tm_today.tm_hour            = 0;
  tm_today.tm_min             = 0;
  tm_today.tm_sec             = 0;
  v_arguments->date           = timegm(&tm_today);
  DAY_DECR(v_arguments->date);
  DAY_INCR(v_arguments->date);
  return 0;
}

int quit(ARGS** _)
{
  (void)_;
  endwin();
  return 1;
}
