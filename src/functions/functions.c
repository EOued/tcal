#include "functions.h"
#include "macro.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int gregorian_zeller(int q, int m, int K, int J)
{

  float div1 = 13.0f * (m + 1) / 5;
  float div2 = K / 4.0f;
  float div3 = J / 4.0f;
  int op1    = floor(div1);
  int op2    = floor(div2);
  int op3    = floor(div3);

  return (q + op1 + K + op2 + op3 - 2 * J) % 7;
}

DAY week_day(float month_day, MONTH month, float year)
{
  float adjusted_month = 1 + month;
  if (adjusted_month < 3) adjusted_month += 12;
  int adjYear = month >= 2 ? year : year - 1;
  int K       = adjYear % 100;
  int J       = floor(adjYear / 100.0f);
  int h       = gregorian_zeller(month_day, adjusted_month, K, J);
  return fmod(h + 5, 7);
}

int* month_day(int month_week_number, DAY _week_day, MONTH m, float year)
{
  int* res;
  MEMCHK((res = calloc(2, sizeof(int))));
  DAY month_first_day = week_day(1.0f, m, year);
  int is_leap =
      (fmod(year, 4) == 0) && (fmod(year, 100) != 0 || fmod(year, 400) == 0);
  int total_month_days[12] = {31, 28 + is_leap, 31, 30, 31, 30,
                              31, 31,           30, 31, 30, 31};
  int day                  = 0;
  if (month_week_number == 0 && _week_day < month_first_day)
  {
    day =
        total_month_days[(m - 1 + 12) % 12] - (month_first_day - _week_day - 1);
    m--;
  }
  else
    day = 7 * month_week_number + (int)_week_day - month_first_day + 1;
  if (month_week_number != 0 && day > total_month_days[m])
  {
    day -= total_month_days[m];
    m++;
  }
  res[0] = day;
  res[1] = m;
  return res;
}
