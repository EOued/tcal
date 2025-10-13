#include "drawer.h"
#include "macro.h"
#include <math.h>
#include <ncurses.h>
#include <string.h>

#define HELP_LENGTH 8

void _help_box(void* varg)
{
  int* ptrpages            = (int*)varg;
  char** text              = calloc(HELP_LENGTH, sizeof(char*));
  char* _text[HELP_LENGTH] = {"?: Show help menu.",
                              "n: Next page/day/week/month.",
                              "p: Previous page/day/week/month.",
                              "h/j/k/l: Vim keybinds for navigation.",
                              "d: Toggle day view.",
                              "w: Toggle week view.",
                              "m: Toggle month view.",
                              "t: Change date to today."};
  uint len;
  for (uint i = 0; i < HELP_LENGTH; i++)
  {
    len     = strlen(_text[i]);
    text[i] = calloc(len + 1, sizeof(char));
    strncpy(text[i], _text[i], len);
  }
  int page = draw_page(COLS / 3, LINES / 3, 2 * COLS / 3, 2 * LINES / 3,
                       "HELP MENU", text, HELP_LENGTH, *ptrpages);
  // Displayed page is not entered argument: most likely an overflow, we set
  // back the page to the latest (provided by draw_page)
  if (page != -1 && page != *ptrpages) *ptrpages = page;
  for (uint i = 0; i < HELP_LENGTH; i++) free(text[i]);
  free(text);
}

void draw_box(uint x1, uint y1, uint x2, uint y2)
{
  move(y1, x1);
  addch(ACS_ULCORNER);
  for (uint _ = 0; _ < (x2 - x1) - 1; _++) addch(ACS_HLINE);
  addch(ACS_URCORNER);

  for (uint y = y1 + 1; y < y2; y++)
  {
    mvaddch(y, x1, ACS_VLINE);
    for (uint x = x1 + 1; x < x2; x++) mvaddch(y, x, ' ');
    mvaddch(y, x2, ACS_VLINE);
  }
  move(y2, x1);
  addch(ACS_LLCORNER);
  for (uint _ = 0; _ < (x2 - x1) - 1; _++) addch(ACS_HLINE);
  addch(ACS_LRCORNER);
  return;
}
