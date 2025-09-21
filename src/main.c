#include "drawer.h"
#include "functions.h"
#include "macro.h"
#include "renderer.h"
#include "views_handling.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

// 293 lines

/* static inline int do_skip_day(int day, MONTH m, int year) */
/* { */
/*   return week_day(day, m, year) > fri; */
/* } */

/* static inline int do_skip_week(int week_number, MONTH m, int year) */
/* { */
/*   int is_a_day_in_good_month = 0; */
/*   int* md; */
/*   for (int i = mon; i < sat; i++) */
/*   { */
/*     md = month_day(week_number, i, m, year); */
/*     is_a_day_in_good_month |= (int)m == md[1]; */
/*     free(md); */
/*   } */
/*   return !is_a_day_in_good_month; */
/* } */
void quit_text(void* _)
{
  (void)_;
  move(LINES - 1, 0);
  addstr("Press q to exit, ? for help...");
  move(0, 0);
}

int main(void)
{
  initscr();
  noecho();
  curs_set(0);
  start_color();
  // int opened_help = 0;
  renderable* r = initRenderable();
  int box_uuid  = -1;
  renderableAdd(r, quit_text, NULL);
  /* void (*view_array[3])(void*) = {day_grid, week_grid, month_grid}; */
  /* uint view_index              = 0; */

  MEMCREATE(int*, box_args, malloc(4 * sizeof(int)));
  MEMCREATE(int*, day_args, calloc(3, sizeof(int)));
  MEMCREATE(int*, week_args, calloc(4, sizeof(int)));
  MEMCREATE(int*, month_args, calloc(3, sizeof(int)));

  // Get current month and year
  time_t t          = time(NULL);
  struct tm* tminfo = localtime(&t);

  struct tm first_day = *tminfo;
  first_day.tm_mday   = 1;
  mktime(&first_day);

  _MONTH(tminfo, month_args[1]);
  _YEAR(tminfo, month_args[2]);

  _WEEK(tminfo, first_day, week_args[1]);
  _MONTH(tminfo, week_args[2]);
  _YEAR(tminfo, week_args[3]);

  _DAY(tminfo, day_args[0]);
  _MONTH(tminfo, day_args[1]);
  _YEAR(tminfo, day_args[2]);

  uint help_page = 0;

  views* v            = viewsInit();
  enum views view     = day;
  enum views old_view = none;
  // Day view
  createView(v, day);
  // Help view
  createView(v, help);

  MEMCREATE(help_arg*, helpActionArgs, calloc(1, sizeof(help_arg)));
  helpActionArgs->view      = &view;
  helpActionArgs->old_view  = &old_view;
  helpActionArgs->r         = r;
  helpActionArgs->uuid      = &box_uuid;
  helpActionArgs->args      = &help_page;
  helpActionArgs->_help_box = _help_box;

  viewsAddAction(v, day, '?', helpViewOpen, &helpActionArgs);
  viewsAddAction(v, day, 'q', quit, NULL);

  viewsAddAction(v, help, '?', helpViewQuit, &helpActionArgs);
  viewsAddAction(v, help, 'q', helpViewQuit, &helpActionArgs);
  viewsAddAction(v, help, 'p', helpViewPreviousAction, &helpActionArgs);
  viewsAddAction(v, help, 'n', helpViewNextAction, &helpActionArgs);

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
      int arg = viewsExecuteAction(v, view, ch);
      if (arg) goto leave;
      RENDER_BREAK(to_render);
    }
    }
    if (to_render)
    {
      RENDER(r);
      to_render = 0;
    }
  }
  /* switch (ch) */
  /* { */
  /* case 'q': */
  /*   if (opened_help) */
  /*   { */
  /*     renderableRemove(r, box_uuid); */
  /*     opened_help = 0; */
  /*     RENDER_BREAK(to_render); */
  /*   } */
  /*   endwin(); */
  /*   goto leave; */
  /* case 'j': */
  /*   if (view_index == 1 && week_args[0] < 4) week_args[0]++; */
  /*   if (view_index == 2 && month_args[0] < 19) month_args[0] += 5; */
  /*   RENDER_BREAK(to_render); */
  /* case 'k': */
  /*   if (view_index == 1 && week_args[0] > 0) week_args[0]--; */
  /*   if (view_index == 2 && month_args[0] > 4) month_args[0] -= 5; */
  /*   RENDER_BREAK(to_render); */
  /* case 'l': */
  /*   if (view_index == 1 && week_args[0] < 4) week_args[0]++; */
  /*   if (view_index == 2 && month_args[0] < 24) month_args[0]++; */
  /*   RENDER_BREAK(to_render); */
  /* case 'h': */
  /*   if (view_index == 1 && week_args[0] > 0) week_args[0]--; */
  /*   if (view_index == 2 && month_args[0] > 0) month_args[0]--; */
  /*   RENDER_BREAK(to_render); */
  /* case 'v': */
  /*   view_index += 1; */
  /*   view_index %= 3; */
  /*   UPDATE_VIEW(view_index, r, view_uuid); */
  /*   if (view_index == 1) updateArgument(r, view_uuid, week_args); */
  /*   if (view_index == 2) updateArgument(r, view_uuid, month_args); */
  /*   RENDER_BREAK(to_render); */
  /* case ' ': */
  /*   view_index = 0; */
  /*   UPDATE_VIEW(view_index, r, view_uuid); */
  /*   RENDER_BREAK(to_render); */
  /* case 'n': */
  /*   if (opened_help) */
  /*   { */
  /*     viewsExecuteAction(v, 0, 'n', helpActionArgs); */
  /*     RENDER_BREAK(to_render); */
  /*   } */
  /*   if (view_index == 2) */
  /*   { */
  /*     CASC_MONTH_INCR(month_args[1], month_args[2]); */
  /*     RENDER_BREAK(to_render); */
  /*   } */
  /*   if (view_index == 1) */
  /*   { */
  /*     do { */
  /*       CASC_WEEK_INCR(week_args[1], week_args[2], week_args[3]); */
  /*     } while (do_skip_week(week_args[1], week_args[2], week_args[3])); */
  /*     RENDER_BREAK(to_render); */
  /*   } */
  /*   if (view_index == 0) */
  /*   { */
  /*     do { */
  /*       CASC_DAY_INCR(day_args[0], day_args[1], day_args[2]); */
  /*     } while (do_skip_day(day_args[0], day_args[1], day_args[2])); */
  /*     RENDER_BREAK(to_render); */
  /*   } */
  /*   break; */
  /* case 'p': */
  /*   if (opened_help && help_page > 0) */
  /*   { */
  /*     help_page--; */
  /*     updateArgument(r, box_uuid, &help_page); */
  /*     RENDER_BREAK(to_render); */
  /*   } */
  /*   if (view_index == 2) */
  /*   { */
  /*     CASC_MONTH_DECR(month_args[1], month_args[2]); */
  /*     MONTH_LIMIT(month_args[1], month_args[2]); */
  /*     RENDER_BREAK(to_render); */
  /*   } */
  /*   if (view_index == 1) */
  /*   { */
  /*     do { */
  /*       CASC_WEEK_DECR(week_args[1], week_args[2], week_args[3]); */
  /*       WDMONTH_LIMIT(week_args[1], week_args[2], week_args[3]) */
  /*     } while (do_skip_week(week_args[1], week_args[2], week_args[3])); */

  /*     RENDER_BREAK(to_render); */
  /*   } */
  /*   if (view_index == 0) */
  /*   { */
  /*     do { */
  /*       CASC_DAY_DECR(day_args[0], day_args[1], day_args[2]); */
  /*       WDMONTH_LIMIT(day_args[0], day_args[1], day_args[2]) */
  /*     } while (do_skip_day(day_args[0], day_args[1], day_args[2])); */

  /*     RENDER_BREAK(to_render); */
  /*   } */

  /*   break; */
  /* case KEY_RESIZE: RENDER_BREAK(to_render); */
  /* if (view_index == 2) updateArgument(r, view_uuid, month_args); */
  /* if (view_index == 1) updateArgument(r, view_uuid, week_args); */
  /* if (view_index == 0) updateArgument(r, view_uuid, day_args); */

  /* if (to_render) */
  /* { */
  /*   render(r); */
  /*   to_render = 0; */
  /* } */
leave:
  freeRenderable(r);
  viewsFree(v);
  free(box_args);
  free(day_args);
  free(week_args);
  free(month_args);
  free(helpActionArgs);
  return EXIT_SUCCESS;
}
