#include "views_handling.h"
#include "functions.h"
#include "macro.h"

view* viewInit(void)
{
  MEMCREATE(view*, v, calloc(1, sizeof(view)));
  v->actionsMap = fhashmapInit();
  v->args       = hashmapInit();
  return v;
}

void _null(void* _)
{
  (void)_;
  return;
}

void viewFree(view* v)
{
  fhashmapFree(v->actionsMap);
  hashmapFree(v->args, _null);
  free(v);
  return;
}

void _viewFree(void* v)
{
  view* _v = (view*)v;
  viewFree(_v);
}

void viewAddAction(view* v, char character, int (*action)(ARGS**))
{
  fhashmapInsert(v->actionsMap, (uint)character, (int (*)(void*))action);
  return;
}

int viewExecuteAction(view* v, char character)
{
  int (*action)(void*) = fhashmapFind(v->actionsMap, (uint)character);
  void* args           = hashmapFind(v->args, (uint)character);
  if (!action) return -1;
  return action(args);
}

// views

views* viewsInit(void)
{
  MEMCREATE(views*, v, calloc(1, sizeof(views)));
  v->views = hashmapInit();
  // top-level view
  createView(v, -1);
  return v;
}

void viewsFree(views* v)
{
  if (!v) return;
  hashmapFree(v->views, _viewFree);
  free(v);
  return;
}

void createView(views* v, uint uuid)
{
  view* _v = viewInit();
  hashmapInsert(v->views, uuid, _v);
  return;
}

void viewsAddAction(views* views, int uuid, char character,
                    int (*action)(ARGS**), ARGS** args)
{
  view* _v = hashmapFind(views->views, uuid);
  viewAddAction(_v, character, action);
  hashmapInsert(_v->args, character, args);
  return;
}

int viewsExecuteAction(views* views, int uuid, char character)
{
  view* _v = hashmapFind(views->views, uuid);
  if (!_v) return 0;
  int action = viewExecuteAction(_v, character);
  if (action == -1)
    return viewExecuteAction(hashmapFind(views->views, -1), character);
  return action;
}
