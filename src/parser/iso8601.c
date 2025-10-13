#include <stdio.h>
#include <time.h>
/*  parses only YYYY-MM-DDTHH:MM:SSZ */
time_t parseiso8601utc(const char* date)
{
  struct tm tt = {0};
  if (sscanf(date, "%4d%2d%2dT%2d%2d00Z", &tt.tm_year, &tt.tm_mon, &tt.tm_mday,
             &tt.tm_hour, &tt.tm_min) != 5)
  {
    return -1;
  }
  tt.tm_mon -= 1;
  tt.tm_year -= 1900;
  tt.tm_isdst = -1;
  return timegm(&tt);
}
