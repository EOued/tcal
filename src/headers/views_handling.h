#ifndef VIEWS_HANDLING
#define VIEWS_HANDLING

#include "functions.h"
#include "macro.h"
#include "renderer.h"

enum views
{
  none,
  help,
  day,
  week,
  month
};

typedef struct
{
  enum views* view;
  enum views* old_view;
  renderable* r;
  int* uuid;
  void* args;
} HELP_ARG;

typedef struct
{
  enum views* view;
  enum views* old_view;
  renderable* r;
  int* uuid;
  void* args;
} DATE_ARG;

typedef struct
{
  fhashmap* actionsMap;
  hashmap* args;
} view;

typedef struct
{
  hashmap* views;
} views;

views* viewsInit(void);
void viewsFree(views* v);
void createView(views* v, uint uuid);
void viewsAddAction(views* views, uint uuid, char character,
                    int (*action)(void*), void* args);
int viewsExecuteAction(views* views, uint uuid, char character);

// views_funcs

int helpViewNextAction(void* varg);
int helpViewPreviousAction(void* varg);
int helpViewOpen(void* varg);
int helpViewQuit(void* varg);
int dayViewNext(void* varg);
int dayNext(void* varg);
int weekViewNext(void* varg);
int monthViewNext(void* varg);
int dayPrevious(void* varg);
int dayToday(void* varg);
int weekNext(void* varg);
int weekPrevious(void* varg);
int monthNext(void* varg);
int monthPrevious(void* varg);
int quit(void* _);
#endif
