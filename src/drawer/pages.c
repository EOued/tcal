#include "drawer.h"
#include "macro.h"
#include <math.h>
#include <ncurses.h>
#include <string.h>

int draw_page(uint x1, uint y1, uint x2, uint y2, char* title, char** lines,
              uint nlines, uint page)
{
  if (!lines) return -1;
  uint nline;
  uint width     = x2 - x1;
  int box_length = y2 - y1 - 3;
  uint pages     = ceil((float)nlines / box_length);
  // Current page is overflowing: resnapping it to the latest page
  if (page >= pages) page = pages - 1;
  draw_box(x1, y1, x2, y2);
  mvprintw(y1 + 1, x1 + 2, "%s", title);
  mvprintw(y2 - 1, x1 + 1, "page %d/%d (n)ext, (p)revious", page + 1, pages);
  int shift = page * box_length;

  for (uint i = shift; i < MIN((page + 1) * box_length, nlines); i++)
  {
    nline = strlen(lines[i]);
    if (nline < width - 2) goto display;
    if (lines[i] && (uint)strlen(lines[i]) >= width) lines[i][width - 3] = '\0';
    lines[i][width - 4] = '.';
    lines[i][width - 5] = '.';
    lines[i][width - 6] = '.';
  display:
    mvprintw(y1 + 2 + (i - shift), x1 + 2, "%s", lines[i]);
  }
  return page;
}
