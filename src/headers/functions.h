#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>

#define MONTH enum _month
#define DAY enum _day

enum _month
{
  jan,
  feb,
  mar,
  apr,
  may,
  jun,
  jul,
  aug,
  sep,
  oct,
  nov,
  dec
};

enum _day
{
  mon,
  tue,
  wes,
  thu,
  fri,
  sat,
  sun
};

int* month_day(int month_week_number, DAY _week_day, MONTH m, float year);
DAY week_day(float month_day, MONTH month, float year);

#endif
