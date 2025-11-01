#ifndef DRAWER_H
#define DRAWER_H

#include "calendar.h"
#include "macro.h"
#include "views_handling.h"

typedef struct
{
  time_t date;
  event_list* e_list;
} view_arguments;

typedef struct
{
  int help_page;
  elist* elist;
  enum views valid_view;
} help_arguments;

void day_grid(void* _);
void week_grid(void* _);
void month_grid(void* _);
void _help_box(void*);
int draw_page(uint x1, uint y1, uint x2, uint y2, char* title, char** lines,
              uint nlines, uint page);
void draw_box(uint x1, uint y1, uint x2, uint y2);
void draw_event(void* varg);
#endif
