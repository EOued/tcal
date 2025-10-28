#ifndef VIEWS_HANDLING
#define VIEWS_HANDLING

#include "functions.h"
#include "list.h"
#include "macro.h"
#include "renderer.h"

enum views
{
  none,
  help,
  day,
  week,
  month,
  event_view
};

typedef struct
{
  list* view;
  renderable* r;
  int* uuid;
  void* args;
} ARGS;

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
void viewsAddAction(views* views, int uuid, char character,
                    int (*action)(ARGS**), ARGS** args);
int viewsExecuteAction(views* views, int uuid, char character);

// views_funcs

int helpViewNextAction(ARGS** args);
int helpViewPreviousAction(ARGS** args);
int helpViewOpen(ARGS** args);
int helpViewQuit(ARGS** args);
int drawEventsOpen(ARGS** args);
int drawEventsQuit(ARGS** args);
int dayView(ARGS** args);
int weekView(ARGS** args);
int monthView(ARGS** args);
int dayNext(ARGS** args);
int dayPrevious(ARGS** args);
int dayToday(ARGS** args);
int weekNext(ARGS** args);
int weekPrevious(ARGS** args);
int monthNext(ARGS** args);
int monthPrevious(ARGS** args);
int currentDate(ARGS** args);
int quit(ARGS** _);
#endif
