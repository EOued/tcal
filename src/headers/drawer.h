#ifndef DRAWER_H
#define DRAWER_H

#include "calendar.h"
#include "macro.h"

typedef struct
{
  int* date;
  calendar* cal_list;
  uint cal_list_capacity;
  uint cal_list_size;
} view_arguments;

void day_grid(void* _);
void week_grid(void* _);
void month_grid(void* _);
void _help_box(void*);
int draw_page(uint x1, uint y1, uint x2, uint y2, char* title, char** lines,
              uint nlines, uint page);
void draw_box(uint x1, uint y1, uint x2, uint y2);
#endif
