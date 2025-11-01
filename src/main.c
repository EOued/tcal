#include "calendar.h"
#include "drawer.h"
#include "functions.h"
#include "list.h"
#include "macro.h"
#include "renderer.h"
#include "views_handling.h"
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

// 293 lines

void quit_text(void* _)
{
  (void)_;
  move(LINES - 1, 0);
  addstr("Press q to exit, ? for help...");
  move(0, 0);
}

static void void_free(void* _)
{
  (void)_;
  return;
}

int main(void)
{
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(0);
  start_color();
  renderable* r = initRenderable();
  int box_uuid  = -1;
  renderableAdd(r, quit_text, NULL);

  MEMCREATE(int*, box_args, malloc(5 * sizeof(int)));
  MEMCREATE(view_arguments*, view_args, calloc(1, sizeof(view_arguments)));
  MEMCREATE(help_arguments*, help_args, calloc(1, sizeof(help_arguments)));
  help_args->help_page = 0;

  time_t now         = time(NULL);
  struct tm tm_today = *localtime(&now);
  // override for testing purposes
  // dec
  tm_today.tm_mon = 11;
  // eighth
  tm_today.tm_mday = 5;
  tm_today.tm_hour = 0;
  tm_today.tm_min  = 0;
  tm_today.tm_sec  = 0;

  view_args->date   = timegm(&tm_today);
  view_args->e_list = initCalendarTemplate();
  // Skips to next available day (useful if launched on a weekend)

  char buffer[100];
  strftime(buffer, sizeof(buffer), "%A %d %B %Y, %H:%M:%S", &tm_today);

  DAY_DECR(view_args->date);
  DAY_INCR(view_args->date);
  int view_uuid = renderableAdd(r, day_grid, view_args);

  views* v = viewsInit();

  list* views_stack = initList(sizeof(enum views));
  enum views view   = day;
  insertElement(views_stack, &view);
  for (enum views _v = help; _v <= event_view; _v++) createView(v, _v);

  MEMCREATE(ARGS*, helpActionArg, calloc(1, sizeof(ARGS)));
  helpActionArg->view = views_stack;
  helpActionArg->r    = r;
  helpActionArg->uuid = &box_uuid;
  helpActionArg->args = help_args;

  MEMCREATE(ARGS*, dateActionArg, calloc(1, sizeof(ARGS)));
  dateActionArg->view = views_stack;
  dateActionArg->r    = r;
  dateActionArg->uuid = &view_uuid;
  dateActionArg->args = view_args;

  elist* elist     = presetActionList(v, dateActionArg, helpActionArg);
  help_args->elist = elist;
  RENDER(r);
  int to_render = 0;
  while (1)
  {
    refresh();
    int ch = getch();
    switch (ch)
    {
    case KEY_RESIZE: RENDER_BREAK(to_render);
    default:
    {
      // -1: Action not found in current level/toplevel
      // 0: Action found, executed successfully
      // 1: Action found, close window
      int arg = viewsExecuteAction(
          v, *(enum views*)RZBL_L_ELEM(views_stack, views_stack->size - 1), ch);
      // Display help
      if (arg == -1 && ch == '?')
      {
        ((help_arguments*)helpActionArg->args)->valid_view =
            *RZBL_L_ELEM(helpActionArg->view, helpActionArg->view->size - 1);
        *helpActionArg->uuid =
            renderableAdd(helpActionArg->r, _help_box, helpActionArg->args);
        enum views v = help;
        insertElement(helpActionArg->view, &v);
      }
      if (arg > 0) goto leave;
      RENDER_BREAK(to_render);
    }
    }
    if (to_render)
    {
      RENDER(r);
      to_render = 0;
    }
  }
leave:
  quit(NULL);
  freeRenderable(r);
  viewsFree(v);
  freeActionList(elist);
  freeList(views_stack, void_free);
  free(box_args);
  freeEventList(view_args->e_list);
  free(view_args);
  free(helpActionArg);
  free(dateActionArg);
  return EXIT_SUCCESS;
}
