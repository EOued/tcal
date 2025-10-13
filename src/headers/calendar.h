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
} event;

typedef struct
{
  event* e;
  uint capacity;
  uint size;
} event_list;

void freeEventList(event_list* l);
event initCalendar(char* dtstart, char* dtend, char* summary, char* description,
                   char* location);
void printCal(event c);
void freeCalendar(event c);
int is_same_day(time_t t1, time_t t2);
int compare_cal(const void* c1, const void* c2);
event_list* initCalendarTemplate(void);
#endif
