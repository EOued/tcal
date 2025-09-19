#ifndef MACROS_H
#define MACROS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ERRMSG(msg)                                                            \
  do {                                                                         \
    fprintf(stderr, #msg);                                                     \
    exit(1);                                                                   \
  } while (0)

#define ERRCHK(op)                                                             \
  do {                                                                         \
    if ((op) == -1) ERRMSG(op);                                                \
  } while (0)

#define MEMCHK(op)                                                             \
  do {                                                                         \
    if ((op) == NULL) ERRMSG(op);                                              \
  } while (0)

#define MEMCREATE(type, var, alloc)                                            \
  type var;                                                                    \
  MEMCHK((var = alloc));

#define REALLOC(size, capacity, size_t, ptr)                                   \
  do {                                                                         \
    if (capacity <= size)                                                      \
    {                                                                          \
      if (!capacity) capacity = 2;                                             \
      else                                                                     \
        capacity *= 2;                                                         \
      MEMCHK(ptr = realloc(ptr, capacity * size_t));                           \
    }                                                                          \
  } while (0)

#define RENDER(renderable)                                                     \
  do {                                                                         \
    erase();                                                                   \
    render(renderable);                                                        \
    refresh();                                                                 \
  } while (0)

#define MAX(a, b) (a > b ? a : b)

#define MIN(a, b) (a < b ? a : b)

#define HLINE_BOXSPLIT(x1, x2, y)                                              \
  do {                                                                         \
    mvaddch(y, x1, ACS_LTEE);                                                  \
    for (int x = x1 + 1; x < x2; x++) mvaddch(y, x, ACS_HLINE);                \
    mvaddch(y, x2, ACS_RTEE);                                                  \
  } while (0)

#define ndgit(x)                                                               \
  ((x) == 0                                                                    \
       ? 1                                                                     \
       : ((x) < 0 ? (int)floor(log10(-(x))) + 1 : (int)floor(log10(x)) + 1))

#define RENDER_BREAK(var)                                                      \
  var = 1;                                                                     \
  break;

#define UPDATE_VIEW(view_index, renderable, view_uuid)                         \
  do {                                                                         \
    renderableRemove(renderable, view_uuid);                                   \
    view_uuid = renderableAdd(r, view_array[view_index], NULL);                \
  } while (0)

#define _DAY(tminfo, day) day = tminfo->tm_mday
#define _WEEK(tminfo, fday, week)                                              \
  week = (tminfo->tm_mday + fday.tm_wday - 1) / 7
#define _MONTH(tminfo, month) month = tminfo->tm_mon
#define _YEAR(tminfo, year) year = tminfo->tm_year + 1900

#define CASC_MONTH_INCR(month, year)                                           \
  month = (month + 1) % 12;                                                    \
  if (!month) year++;

#define CASC_MONTH_DECR(month, year)                                           \
  month = (month - 1 + 12) % 12;                                               \
  if (!month == 11) year--;

#define CASC_WEEK_INCR(week, month, year)                                      \
  week = (week + 1) % 5;                                                       \
  if (!week) CASC_MONTH_INCR(month, year);

#define CASC_WEEK_DECR(week, month, year)                                      \
  week = (week - 1 + 5) % 5;                                                   \
  if (week == 4) CASK_MONTH_DECR(month, year);

#define CASC_DAY_INCR

#define uint unsigned int
#endif
