#include "calendar.h"
#include "drawer.h"
#include "functions.h"
#include "macro.h"
#include <math.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>

void day_grid(void* varg)
{
  view_arguments* _args   = (view_arguments*)varg;
  time_t args             = _args->date;
  int day                 = localtime(&args)->tm_mday;
  int month               = localtime(&args)->tm_mon;
  int year                = localtime(&args)->tm_year + 1900;
  char* tday[7]           = {" Monday", " Tuesday",  " Wednesday", " Thursday",
                             " Friday", " Saturday", " Sunday"};
  char* month_display[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                             "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  DAY _week_day           = ISO_ZELLER(day, (MONTH)month, year);
  mvprintw(0, COLS / 2, "%s, %d %s %d", tday[_week_day], day,
           month_display[month], year);
  draw_box(0, 1, COLS - 1, LINES - 2);

  qsort(_args->cal_list, _args->cal_list_size, sizeof(calendar), compare_cal);
  // Iterate until date have been skipped
  int passed_date = 0;
  uint index      = 0;
  uint y          = 3;
  while (index < _args->cal_list_size)
  {
    calendar c = _args->cal_list[index];
    if (!is_same_day(args, c.start))
    {
      index++;
      if (passed_date) break;
      continue;
    }
    if (!passed_date) passed_date = 1;
    mvprintw(y, 4, "%s | %s | %s", c.summary, c.description, c.location);
    y++;
    index++;
  }
}

void week_grid(void* varg)
{
  view_arguments* _args   = (view_arguments*)varg;
  time_t args             = _args->date;
  int day                 = localtime(&args)->tm_mday;
  int month               = localtime(&args)->tm_mon;
  int year                = localtime(&args)->tm_year + 1900;
  int _week               = week_nbr(day, month, year);
  int index               = ISO_ZELLER(day, (MONTH)month, year);
  char* month_display[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                             "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  mvprintw(0, COLS / 2 - 7, "Week %d - %s, %d", _week + 1, month_display[month],
           year);
  // Each day
  double step   = COLS / 5;
  char* days[5] = {" Monday", " Tuesday", " Wednesday", " Thursday", " Friday"};

  int* dm;
  for (int k = 0; k < 5; k++)
  {
    double start = k * step;
    double end   = (k + 1) * step - 1;
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_color(COLOR_WHITE + 1, 500, 500, 500);
    init_pair(2, COLOR_WHITE + 1, COLOR_BLACK);
    if (k == index) attron(COLOR_PAIR(1));
    draw_box(start, 2, end, LINES - 3);
    HLINE_BOXSPLIT(start, end, 4);
    attroff(COLOR_PAIR(1));

    dm = month_day(_week, (DAY)k, month, year);
    if (dm[1] != month) attron(COLOR_PAIR(2));
    mvprintw(3, (COLS / 10) + start - strlen(days[k]) / 2, "%s, %s %d",
             month_display[dm[1]], days[k], dm[0]);
    attroff(COLOR_PAIR(2));
    free(dm);
  }
}

void month_grid(void* varg)
{
  view_arguments* _args   = (view_arguments*)varg;
  time_t args             = _args->date;
  int day                 = localtime(&args)->tm_mday;
  int month               = localtime(&args)->tm_mon;
  int year                = localtime(&args)->tm_year + 1900;
  int _week               = week_nbr(day, month, year);
  int index               = 5 * _week + ISO_ZELLER(day, (MONTH)month, year);
  char* month_display[12] = {"January",   "February", "March",    "April",
                             "May",       "June",     "July",     "August",
                             "September", "October",  "November", "December"};
  mvprintw(0, COLS / 2 - 7, "%s, %d", month_display[month], year);
  double vstep  = COLS / 5;
  double hstep  = (LINES - 2) / 5;
  char* days[5] = {" Monday", " Tuesday", " Wednesday", " Thursday", " Friday"};
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
                   (strlen(days[k]) + 2 + ndgit(k + k2 * 5)) / 2,
               "%s, %d", days[k], dm[0]);
      attroff(COLOR_PAIR(2));
      free(dm);
    }
  }
}
