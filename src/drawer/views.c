#include "drawer.h"
#include "functions.h"
#include "macro.h"
#include <ncurses.h>
#include <string.h>

void day_grid(void* _)
{
  (void)_;
  mvprintw(0, COLS / 2, "Day %d", 0);
  draw_box(0, 1, COLS - 1, LINES - 2);
}

void week_grid(void* varg)
{
  int* ptrindex = (int*)varg;
  mvprintw(0, COLS / 2 - 7, "Week number %d", 0);
  // Each day
  double step  = COLS / 5;
  char* day[5] = {" Monday", " Tuesday", " Wednesday", " Thursday", " Friday"};
  for (int k = 0; k < 5; k++)
  {
    double start = k * step;
    double end   = (k + 1) * step - 1;
    init_pair(1, COLOR_RED, COLOR_BLACK);
    if (k == *ptrindex) attron(COLOR_PAIR(1));
    draw_box(start, 2, end, LINES - 3);
    HLINE_BOXSPLIT(start, end, 4);
    attroff(COLOR_PAIR(1));
    mvprintw(3, (COLS / 10) + start - strlen(day[k]) / 2, "%s", day[k]);
  }
}

void month_grid(void* varg)
{
  int* args               = (int*)varg;
  int index               = args[0];
  int month               = args[1];
  int year                = args[2];
  char* month_display[12] = {"January",   "February", "March",    "April",
                             "May",       "June",     "July",     "August",
                             "September", "October",  "November", "December"};
  mvprintw(0, COLS / 2 - 7, "%s, %d", month_display[month], year);
  double vstep = COLS / 5;
  double hstep = (LINES - 2) / 5;
  char* day[5] = {" Monday", " Tuesday", " Wednesday", " Thursday", " Friday"};
  int* dm;
  for (int k = 0; k < 5; k++)
  {
    double start1 = k * vstep;
    double end1   = (k + 1) * vstep - 1;
    for (int k2 = 0; k2 < 5; k2++)
    {
      double start2 = 1 + k2 * hstep;
      double end2   = 1 + (k2 + 1) * hstep - 1;

      init_pair(1, COLOR_RED, COLOR_BLACK);
      init_color(COLOR_WHITE + 1, 500, 500, 500);
      init_pair(2, COLOR_WHITE + 1, COLOR_BLACK);
      if (k2 * 5 + k == index) attron(COLOR_PAIR(1));
      draw_box(start1, start2, end1, end2);
      HLINE_BOXSPLIT(start1, end1, start2 + 2);
      attroff(COLOR_PAIR(1));

      dm = month_day(k2, (DAY)k, (MONTH)month, year);
      if (dm[1] != month) attron(COLOR_PAIR(2));

      mvprintw(start2 + 1,
               (COLS / 10) + start1 -
                   (strlen(day[k]) + 2 + ndgit(k + k2 * 5)) / 2,
               "%s, %d", day[k], dm[0]);
      attroff(COLOR_PAIR(2));
      free(dm);
    }
  }
}
