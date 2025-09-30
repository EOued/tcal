#include "functions.h"
#include "macro.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int week_nbr(int day, MONTH m, int year)
{
  DAY month_first_day = ISO_ZELLER(1, m, year);
  return (day + month_first_day - 1) / 7;
}

int* month_day(int month_week_number, DAY _week_day, MONTH m, int year)
{
  int* res;
  MEMCHK((res = calloc(2, sizeof(int))));
  DAY month_first_day = ISO_ZELLER(1, m, year);
  uint day            = 0;
  if (month_week_number == 0 && _week_day < month_first_day)
  {
    day = TOTAL_MONTH_DAY((m - 1 + 12) % 12, year) -
          (month_first_day - _week_day - 1);
    m--;
  }
  else
    day = 7 * month_week_number + (int)_week_day - month_first_day + 1;
  if (month_week_number != 0 && day > TOTAL_MONTH_DAY(m, year))
  {
    day -= TOTAL_MONTH_DAY(m, year);
    m++;
  }
  res[0] = day;
  res[1] = m;
  return res;
}
