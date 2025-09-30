#include "calendar.h"
#include "iso8601.h"
#include "string.h"
#include <time.h>

calendar initCalendar(char* dtstart, char* dtend, char* summary,
                      char* description, char* location)
{
  int n1     = strlen(summary) + 1;
  int n2     = strlen(description) + 1;
  int n3     = strlen(location) + 1;
  calendar c = {.summary     = calloc(n1, sizeof(char)),
                .description = calloc(n2, sizeof(char)),
                .location    = calloc(n3, sizeof(char))};

  strncpy(c.summary, summary, n1);
  strncpy(c.description, description, n2);
  strncpy(c.location, location, n3);
  c.start = parseiso8601utc(dtstart);
  c.end   = parseiso8601utc(dtend);

  return c;
}

time_t date(int day, MONTH month, int year)
{
  struct tm tt = {0};
  tt.tm_mday   = day;
  tt.tm_mon    = month;
  tt.tm_year   = year - 1900;
  tt.tm_isdst  = -1;
  return mktime(&tt) - timezone;
}

int date_index(time_t t1)
{
  struct tm tm1 = *localtime(&t1);
  return 372 * tm1.tm_year + 31 * tm1.tm_mon + tm1.tm_mday;
}

int compare_cal(const void* c1, const void* c2)
{
  return date_index(((calendar*)c1)->start - ((calendar*)c2)->start);
}
int is_same_day(time_t t1, time_t t2)
{
  struct tm tm1 = *localtime(&t1);
  struct tm tm2 = *localtime(&t2);

  return (tm1.tm_year == tm2.tm_year && tm1.tm_mon == tm2.tm_mon &&
          tm1.tm_mday == tm2.tm_mday);
}

void freeCalendar(calendar c)
{
  free(c.summary);
  free(c.description);
  free(c.location);
  return;
}
