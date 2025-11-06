#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "macro.h"
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

int* month_day(int month_week_number, DAY _week_day, MONTH m, int year);
int week_nbr(int day, MONTH m, int year);

// fHASHMAP

struct fHASH_NODE
{
  int key;
  int (*element)(void*);
  struct fHASH_NODE* next;
};

typedef struct
{
  struct fHASH_NODE* start;
} fhashmap;

fhashmap* fhashmapInit(void);
void fhashmapFree(fhashmap* map);
void fhashmapInsert(fhashmap* map, int key, int (*element)(void*));
int (*fhashmapFind(fhashmap* map, int key))(void*);

// HASHMAP

struct HASH_NODE
{
  int key;
  void* element;
  struct HASH_NODE* next;
};

typedef struct
{
  struct HASH_NODE* start;
} hashmap;

hashmap* hashmapInit(void);
void hashmapFree(hashmap* map, void (*freeFunc)(void*));
void hashmapInsert(hashmap* map, int key, void* element);
void* hashmapFind(hashmap* map, int key);
void hashmapDelete(hashmap* map, int key, void (*freeFunc)(void*));

#endif
