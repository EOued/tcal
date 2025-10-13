#include "calendar.h"
#include "iso8601.h"
#include "string.h"
#include <time.h>

void freeEventList(event_list* l)
{
  for (uint i = 0; i < l->size; i++) freeCalendar(l->e[i]);
  free(l->e);
  free(l);
  return;
}

event initCalendar(char* dtstart, char* dtend, char* summary, char* description,
                   char* location)
{
  int n1  = strlen(summary) + 1;
  int n2  = strlen(description) + 1;
  int n3  = strlen(location) + 1;
  event c = {.summary     = calloc(n1, sizeof(char)),
             .description = calloc(n2, sizeof(char)),
             .location    = calloc(n3, sizeof(char))};

  strncpy(c.summary, summary, n1);
  strncpy(c.description, description, n2);
  strncpy(c.location, location, n3);
  c.start = parseiso8601utc(dtstart);
  c.end   = parseiso8601utc(dtend);

  return c;
}

void printCal(event c)
{
  printf("Summary: %s\n", c.summary);
  printf("Description: %s\n", c.description);
  printf("Location: %s\n", c.location);
  char buffer[64];
  struct tm* tm_info = localtime(&c.start);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
  printf("dtstart: %s\n", buffer);
  printf("DAY %d\n", tm_info->tm_mday);
  tm_info = localtime(&c.end);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
  printf("dtend: %s\n", buffer);
}

int compare_cal(const void* c1, const void* c2)
{
  time_t t1 = *(const time_t*)c1;
  time_t t2 = *(const time_t*)c2;

  if (t1 < t2) return -1;
  if (t1 > t2) return 1;
  return 0;
}
int is_same_day(time_t t1, time_t t2)
{
  struct tm tm1 = *localtime(&t1);
  struct tm tm2 = *localtime(&t2);

  return (tm1.tm_year == tm2.tm_year && tm1.tm_mon == tm2.tm_mon &&
          tm1.tm_mday == tm2.tm_mday);
}

void freeCalendar(event c)
{
  free(c.summary);
  free(c.description);
  free(c.location);
  return;
}
