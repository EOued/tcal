#include "macro.h"
#include "views_handling.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void addAction(elist* elist, views* views, int view, char character,
               int (*act)(ARGS**), ARGS** args, char* description)
{
  if (!elist || !elist->elements) return;
  REALLOC(elist->size, elist->capacity, sizeof(action), (elist->elements));
  elist->elements[elist->size].views       = views;
  elist->elements[elist->size].view        = view;
  elist->elements[elist->size].character   = character;
  elist->elements[elist->size].action      = act;
  elist->elements[elist->size].args        = args;
  int n                                    = strlen(description);
  elist->elements[elist->size].description = calloc(n + 1, 1);
  strncpy(elist->elements[elist->size].description, description, n);
  elist->size++;
  return;
}

void addActionsFromList(elist* elist)
{
  for (unsigned int i = 0; i < elist->size; i++)
    viewsAddAction(elist->elements[i].views, elist->elements[i].view,
                   elist->elements[i].character, elist->elements[i].action,
                   elist->elements[i].args);
}

void freeActionList(elist* elist)
{
  for (unsigned int i = 0; i < elist->size; i++)
    free(elist->elements[i].description);
  free(elist->elements);
  free(elist);
}

elist* presetActionList(views* v, ARGS** dateActionArg, ARGS** helpActionArg)
{
  elist* el;
  MEMCHK(el = calloc(1, sizeof(elist)));
  el->size     = 0;
  el->capacity = 2;
  MEMCHK(el->elements = calloc(2, sizeof(action)));

  addAction(el, v, -1, 'd', dayView, dateActionArg, "Switches to day view");
  addAction(el, v, -1, 'w', weekView, dateActionArg, "Switches to week view");
  addAction(el, v, -1, 'm', monthView, dateActionArg, "Switches to month view");
  addAction(el, v, -1, 't', currentDate, dateActionArg,
            "Set date to current date");
  addAction(el, v, -1, 'q', quit, NULL, "Quit");

  addAction(el, v, help, 'q', helpViewQuit, helpActionArg, "");
  addAction(el, v, help, '?', helpViewQuit, helpActionArg, "");
  addAction(el, v, help, 'p', helpViewPreviousAction, helpActionArg, "");
  addAction(el, v, help, 'n', helpViewNextAction, helpActionArg, "");

  addAction(el, v, event_view, 'e', drawEventsQuit, dateActionArg,
            QUIT_MENU_DESC);
  addAction(el, v, event_view, 'q', drawEventsQuit, dateActionArg,
            QUIT_MENU_DESC);

  addAction(el, v, day, 'n', dayNext, dateActionArg, NEXT_DAY);
  addAction(el, v, day, 'p', dayPrevious, dateActionArg, PREVIOUS_DAY);
  addAction(el, v, day, 'e', drawEventsOpen, dateActionArg, "Show event menu");

  addAction(el, v, week, 'k', dayNext, dateActionArg, NEXT_DAY);
  addAction(el, v, week, 'l', dayNext, dateActionArg, NEXT_DAY);
  addAction(el, v, week, 'j', dayPrevious, dateActionArg, PREVIOUS_DAY);
  addAction(el, v, week, 'h', dayPrevious, dateActionArg, PREVIOUS_DAY);
  addAction(el, v, week, 'n', weekNext, dateActionArg, NEXT_WEEK);
  addAction(el, v, week, 'p', weekPrevious, dateActionArg, PREVIOUS_WEEK);

  addAction(el, v, month, 'j', weekNext, dateActionArg, NEXT_WEEK);
  addAction(el, v, month, 'l', dayNext, dateActionArg, NEXT_DAY);
  addAction(el, v, month, 'k', weekPrevious, dateActionArg, PREVIOUS_WEEK);
  addAction(el, v, month, 'h', dayPrevious, dateActionArg, PREVIOUS_DAY);
  addAction(el, v, month, 'n', monthNext, dateActionArg, NEXT_MONTH);
  addAction(el, v, month, 'p', monthPrevious, dateActionArg, PREVIOUS_MONTH);

  addActionsFromList(el);
  return el;
}
