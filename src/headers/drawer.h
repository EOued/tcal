#ifndef DRAWER_H
#define DRAWER_H

void day_grid(void* _);
void week_grid(void* _);
void month_grid(void* _);
void _help_box(void*);
void draw_box(unsigned int x1, unsigned int y1, unsigned int x2,
              unsigned int y2, char** text, unsigned int lines);

#endif
