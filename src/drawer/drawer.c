#include "drawer.h"
#include "macro.h"
#include "views_handling.h"
#include <math.h>
#include <ncurses.h>
#include <string.h>

#define HELP_LENGTH 8

void draw_event(void* varg)
{
  (void)varg;
  char** text = calloc(1, sizeof(char*));
  int page    = draw_page(COLS / 3, LINES / 3, 2 * COLS / 3, LINES / 3 + 3, "",
                          text, 1, 0);
  FREE(text);
  (void)page;
}

char** extract_strings(const elist* elist, int* size, int valid_view)
{
  if (!elist || !elist->elements || elist->size == 0) return NULL;
  MEMCREATE(char**, strings, calloc(elist->size, sizeof(char*)));
  *size = elist->size;
  (void)valid_view;
  int n, index;
  for (uint i = 0; i < elist->size; i++)
    if (elist->elements[i].view == -1 || elist->elements[i].view == valid_view)
    {
      index = i - (elist->size - *size);
      n     = strlen(elist->elements[i].description);
      MEMCHK(strings[index] = calloc(n + 4, 1));
      strings[index][0] = elist->elements[i].character;
      strings[index][1] = ':';
      strings[index][2] = ' ';
      strncpy(strings[index] + 3, elist->elements[i].description, n);
    }
    else
      (*size)--;

  return strings;
}

void _help_box(void* args)
{
  help_arguments* help_args = (help_arguments*)args;
  elist* elist              = help_args->elist;
  int* ptrpages             = &help_args->help_page;
  int size                  = 0;
  char** str = extract_strings(elist, &size, help_args->valid_view);
  int page   = draw_page(COLS / 3, LINES / 3, 2 * COLS / 3, 2 * LINES / 3,
                         "HELP MENU", str, size, *ptrpages);
  // Displayed page is not entered argument: most likely an overflow, we set
  // back the page to the latest (provided by draw_page)
  if (page != -1 && page != *ptrpages) *ptrpages = page;
  for (int i = 0; i < size; i++) FREE(str[i]);
  FREE(str);
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
