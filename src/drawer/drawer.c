#include "drawer.h"
#include "macro.h"
#include <math.h>
#include <ncurses.h>
#include <string.h>

#define HELP_LENGTH 21

void _help_box(void* _)
{
  (void)_;
  char** text              = calloc(HELP_LENGTH, sizeof(char*));
  char* _text[HELP_LENGTH] = {
      "?: Show help menu.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "v: Change view.",
      "space: When this key is pressed, switch to day view of selected day."};
  uint len;
  for (uint i = 0; i < HELP_LENGTH; i++)
  {
    len     = strlen(_text[i]);
    text[i] = calloc(len + 1, sizeof(char));
    strncpy(text[i], _text[i], len);
  }
  draw_page(COLS / 3, LINES / 3, 2 * COLS / 3, 2 * LINES / 3, "HELP MENU", text,
            HELP_LENGTH, 1);
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
