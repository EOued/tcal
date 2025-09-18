#ifndef DRAWER_H
#define DRAWER_H

#include "macro.h"

void day_grid(void* _);
void week_grid(void* _);
void month_grid(void* _);
void _help_box(void*);
void draw_page(uint x1, uint y1, uint x2, uint y2, char* title, char** lines,
               uint nlines, uint page);
void draw_box(uint x1, uint y1, uint x2, uint y2);
#endif
