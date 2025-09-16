#include "drawer.h"
#include "macro.h"
#include <math.h>
#include <ncurses.h>
#include <string.h>

#define HELP_LENGTH 3

void _help_box(void* _)
{
  (void)_;
  char* text[HELP_LENGTH] = {"HELP MENU", "?: Show help menu.",
                             "v: Change view."};
  draw_box(COLS / 3, LINES / 3, 2 * COLS / 3, 2 * LINES / 3, text, HELP_LENGTH);
}

void week_grid(void* _)
{
  (void)_;
  mvprintw(0, COLS / 2 - 7, "Week number %d", 0);
  // Each day
  double step  = COLS / 5;
  char* day[5] = {" Monday", " Tuesday", " Wednesday", " Thursday", " Friday"};
  for (int k = 0; k < 5; k++)
  {
    double start = k * step;
    double end   = (k + 1) * step - 1;
    draw_box(start, 2, end, LINES - 3, NULL, 0);
    mvprintw(3, (COLS / 10) + start - strlen(day[k]) / 2, "%s", day[k]);
    HLINE_BOXSPLIT(start, end, 4);
  }
}

void month_grid(void* _)
{
  (void)_;
  mvprintw(0, COLS / 2 - 7, "Month %d", 0);
  double vstep = COLS / 5;
  double hstep = (LINES - 2) / 5;
  char* day[5] = {" Monday", " Tuesday", " Wednesday", " Thursday", " Friday"};
  for (int k = 0; k < 5; k++)
  {
    double start1 = k * vstep;
    double end1   = (k + 1) * vstep - 1;
    for (int k2 = 0; k2 < 5; k2++)
    {
      double start2 = 1 + k2 * hstep;
      double end2   = 1 + (k2 + 1) * hstep - 1;
      draw_box(start1, start2, end1, end2, NULL, 0);
      mvprintw(start2 + 1,
               (COLS / 10) + start1 -
                   (strlen(day[k]) + 2 + ndgit(k + k2 * 5)) / 2,
               "%s, %d", day[k], k2 * 5 + k);
      HLINE_BOXSPLIT(start1, end1, start2 + 2);
    }
  }
}

void draw_box(unsigned int x1, unsigned int y1, unsigned int x2,
              unsigned int y2, char** text, unsigned int lines)
{
  move(y1, x1);
  addch(ACS_ULCORNER);
  for (unsigned int _ = 0; _ < (x2 - x1) - 1; _++) addch(ACS_HLINE);
  addch(ACS_URCORNER);
  unsigned int string_len;
  unsigned int index;
  unsigned int start = x1 + 2;
  char c;
  for (unsigned int y = y1 + 1; y < y2; y++)
  {
    mvaddch(y, x1, ACS_VLINE);
    // Draw text
    index = y - (y1 + 1);
    if (text && index < lines) string_len = strlen(text[index]);
    for (unsigned int x = x1 + 1; x < x2; x++)
    {
      c = ' ';
      if (text && index < lines)
      {
        if (x >= start && x < start + string_len) c = text[index][x - start];
        if (start + string_len > x2 - 5 && x > x2 - 5) c = '.';
      }
      mvaddch(y, x, c);
    }

    mvaddch(y, x2, ACS_VLINE);
  }
  move(y2, x1);
  addch(ACS_LLCORNER);
  for (unsigned int _ = 0; _ < (x2 - x1) - 1; _++) addch(ACS_HLINE);
  addch(ACS_LRCORNER);
  return;
}
