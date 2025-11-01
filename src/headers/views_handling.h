#ifndef VIEWS_HANDLING
#define VIEWS_HANDLING

#include "functions.h"
#include "list.h"
#include "macro.h"
#include "renderer.h"

// Could implement translation capabilities

#define QUIT_MENU_DESC "Quit this menu"
#define NEXT_DAY "Next day"
#define PREVIOUS_DAY "Previous day"
#define NEXT_WEEK "Next week"
#define PREVIOUS_WEEK "Previous week"
#define NEXT_MONTH "Next month"
#define PREVIOUS_MONTH "Previous month"

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

typedef struct
{
  views* views;
  int view;
  char character;
  int (*action)(ARGS**);
  ARGS** args;
  // For help menu
  char* description;
} action;

// Assuming no supressing of elements
typedef struct
{
  action* elements;
  unsigned int size;
  unsigned int capacity;
} elist;

views* viewsInit(void);
void viewsFree(views* v);
void createView(views* v, uint uuid);
void viewsAddAction(views* views, int uuid, char character,
                    int (*action)(ARGS**), ARGS** args);
int viewsExecuteAction(views* views, int uuid, char character);

elist* presetActionList(views* v, ARGS** dateActionArg, ARGS** helpActionArg);
void freeActionList(elist* elist);

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
