#ifndef MACROS_H
#define MACROS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR(msg)                                                               \
  do {                                                                         \
    fprintf(stderr, #msg);                                                     \
    exit(1);                                                                   \
  } while (0)

#define ERRCHK(op)                                                             \
  do {                                                                         \
    if ((op) == -1) ERR(op);                                                   \
  } while (0)

#define MEMCHK(op)                                                             \
  do {                                                                         \
    if ((op) == NULL) ERR(op);                                                 \
  } while (0)

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

#define uint unsigned int
#endif
