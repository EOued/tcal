#ifndef MACROS_H
#define MACROS_H

#include "functions.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ERRMSG(msg)                                                            \
  do                                                                           \
  {                                                                            \
    fprintf(stderr, #msg);                                                     \
    exit(1);                                                                   \
  } while (0)

#define ERRCHK(op)                                                             \
  do                                                                           \
  {                                                                            \
    if ((op) == -1) ERRMSG(op);                                                \
  } while (0)

#define MEMCHK(op)                                                             \
  do                                                                           \
  {                                                                            \
    if ((op) == NULL) ERRMSG(op);                                              \
  } while (0)

#define MEMCREATE(type, var, alloc)                                            \
  type var;                                                                    \
  MEMCHK((var = alloc));

#define FREE(ptr)                                                              \
  if (ptr) free(ptr)

#define REALLOC(size, capacity, size_t, ptr)                                   \
  do                                                                           \
  {                                                                            \
    if (capacity <= size)                                                      \
    {                                                                          \
      if (!capacity) capacity = 2;                                             \
      else                                                                     \
        capacity *= 2;                                                         \
      MEMCHK(ptr = realloc(ptr, capacity * size_t));                           \
    }                                                                          \
  } while (0)

#define RENDER(renderable)                                                     \
  do                                                                           \
  {                                                                            \
    /*    erase();							*/                                                    \
    render(renderable);                                                        \
    /*refresh();*/                                                             \
  } while (0)

#define MIN(a, b) (a < b ? a : b)

#define HLINE_BOXSPLIT(x1, x2, y)                                              \
  do                                                                           \
  {                                                                            \
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

#define UPDATE_VIEW(renderable, view_uuid, view_func, view_args)               \
  do                                                                           \
  {                                                                            \
    renderableRemove(renderable, view_uuid);                                   \
    view_uuid = renderableAdd(renderable, view_func, view_args);               \
  } while (0)

#define IS_LEAP(year)                                                          \
  (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0))
#define FIRST_HALF(m, y)                                                       \
  ((31 * !(m % 2)) + (28 + (IS_LEAP(y)) * (m == 1) + 2 * (m > 1)) * (m % 2))
#define SECOND_HALF(m) (31 * (m % 2) + 30 * !(m % 2))
#define TOTAL_MONTH_DAY(m, y)                                                  \
  ((m < 7) * FIRST_HALF(m, y) + (m >= 7) * SECOND_HALF(m))

#define GREGORIAN_ZELLER(q, m, K, J)                                           \
  (q + (int)floor(13.0f * (m + 1) / 5) + K + (int)floor(K / 4.0f) +            \
   (int)floor(J / 4.0f) + 5 * J) %                                             \
      7

#define ISO_ZELLER(d, m, y)                                                    \
  (GREGORIAN_ZELLER(d, (m + 1) + (m + 1 < 3 ? 12 : 0),                         \
                    (m >= 2 ? y : y - 1) % 100,                                \
                    (int)floor((m >= 2 ? y : y - 1) / 100)) +                  \
   5) %                                                                        \
      7

#define TIME_ZELLER(time)                                                      \
  ISO_ZELLER(localtime(&time)->tm_mday, localtime(&time)->tm_mon,              \
             localtime(&time)->tm_year + 1900)

#define MONTH_INCR(args)                                                       \
  do                                                                           \
  {                                                                            \
    struct tm tm_next = *localtime(&args);                                     \
    tm_next.tm_mday   = 1;                                                     \
    tm_next.tm_mon++;                                                          \
    args = timegm(&tm_next);                                                   \
  } while (0)

#define MONTH_DECR(args)                                                       \
  do                                                                           \
  {                                                                            \
    struct tm tm_next = *localtime(&args);                                     \
    tm_next.tm_mday   = 1;                                                     \
    tm_next.tm_mon--;                                                          \
    args = timegm(&tm_next);                                                   \
  } while (0)

#define DAY_INCR(args)                                                         \
  do                                                                           \
  {                                                                            \
    struct tm tm_next = *localtime(&args);                                     \
    tm_next.tm_mday++;                                                         \
    args = timegm(&tm_next);                                                   \
  } while (TIME_ZELLER(args) > fri)

#define DAY_DECR(args)                                                         \
  do                                                                           \
  {                                                                            \
    struct tm tm_next = *localtime(&args);                                     \
    tm_next.tm_mday--;                                                         \
    args = timegm(&tm_next);                                                   \
  } while (TIME_ZELLER(args) > fri)

#define WEEK_INCR(args)                                                        \
  do                                                                           \
  {                                                                            \
    struct tm tm_next = *localtime(&args);                                     \
    tm_next.tm_mday += 7;                                                      \
    args = timegm(&tm_next);                                                   \
  } while (0)

#define WEEK_DECR(args)                                                        \
  do                                                                           \
  {                                                                            \
    struct tm tm_next = *localtime(&args);                                     \
    tm_next.tm_mday -= 7;                                                      \
    args = timegm(&tm_next);                                                   \
  } while (0)

#define TIME_BAR(time_l, time_s, x1, x2, y)                                    \
  do                                                                           \
  {                                                                            \
    char buff1[6];                                                             \
    HLINE_BOXSPLIT(x1, x2, y);                                                 \
    uint shift = 0;                                                            \
    for (int i = 0; i < time_s; i++)                                           \
    {                                                                          \
      strftime(buff1, sizeof(buff1), "%H:%M", localtime(time_l[i]));           \
      mvprintw(y, x1 + 3 + shift, " %s ", buff1);                              \
      if (i != time_s - 1) mvprintw(y, x1 + 10 + shift, "-");                  \
      shift += 8;                                                              \
    }                                                                          \
  } while (0)

#define uint unsigned int

#define RZBL_L_ELEM(l, pos) ((char*)l->elements + ((pos) * l->esize))

#endif
