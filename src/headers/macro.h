#ifndef MACROS_H
#define MACROS_H

#include "functions.h"
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

#define IS_LEAP(year)                                                          \
  (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0))
#define FIRST_HALF(m, y)                                                       \
  ((31 * !(m % 2)) + (28 + (IS_LEAP(y)) * (m == 1) + 2 * (m > 1)) * (m % 2))
#define SECOND_HALF(m) (31 * (m % 2) + 30 * !(m % 2))
#define TOTAL_MONTH_DAY(m, y)                                                  \
  (m < 7) * FIRST_HALF(m, y) + (m >= 7) * SECOND_HALF(m)

#define CASC_MONTH_INCR(month, year)                                           \
  do {                                                                         \
    month = (month + 1) % 12;                                                  \
    if (!month) year++;                                                        \
  } while (0);

#define CASC_MONTH_DECR(month, year)                                           \
  do {                                                                         \
    month = (month - 1 + 12) % 12;                                             \
    if (month == 11) year--;                                                   \
  } while (0);

#define MONTH_LIMIT(month, year)                                               \
  do {                                                                         \
    if (year < 1900)                                                           \
    {                                                                          \
      year  = 1900;                                                            \
      month = 0;                                                               \
    }                                                                          \
  } while (0);

#define WDMONTH_LIMIT(other, month, year)                                      \
  do {                                                                         \
    if (year < 1900)                                                           \
    {                                                                          \
      year  = 1900;                                                            \
      month = 0;                                                               \
      other = 0;                                                               \
    }                                                                          \
  } while (0);

#define CASC_WEEK_INCR(week, month, year)                                      \
  do {                                                                         \
    week = (week + 1) % 5;                                                     \
    if (!week) CASC_MONTH_INCR(month, year);                                   \
  } while (0);

#define CASC_WEEK_DECR(week, month, year)                                      \
  do {                                                                         \
    week = (week - 1 + 5) % 5;                                                 \
    if (week == 4) CASC_MONTH_DECR(month, year);                               \
  } while (0);

#define CASC_DAY_INCR(day, month, year)                                        \
  do {                                                                         \
    if (++day >= TOTAL_MONTH_DAY(month, year))                                 \
    {                                                                          \
      day = 0;                                                                 \
      CASC_MONTH_INCR(month, year);                                            \
    }                                                                          \
  } while (0);

#define CASC_DAY_DECR(day, month, year)                                        \
  do {                                                                         \
    if (--day < 0)                                                             \
    {                                                                          \
      CASC_MONTH_DECR(month, year);                                            \
      day = TOTAL_MONTH_DAY(month, year);                                      \
    }                                                                          \
  } while (0);

#define uint unsigned int
#endif
