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

#define UPDATE_VIEW(renderable, view_uuid, view_func, view_args)               \
  do {                                                                         \
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

#define uint unsigned int
#endif
