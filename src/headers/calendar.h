#ifndef CALENDAR_H
#define CALENDAR_H

#include "functions.h"
#include "macro.h"
#include <time.h>

typedef struct
{
  time_t start;
  time_t end;
  char* summary;
  char* description;
  char* location;
} calendar;

calendar initCalendar(char* dtstart, char* dtend, char* summary,
                      char* description, char* location);
void freeCalendar(calendar c);
time_t date(int day, MONTH month, int year);
int is_same_day(time_t t1, time_t t2);
int compare_cal(const void* c1, const void* c2);

#endif
