#include "renderer.h"
#include "list.h"
#include "stdlib.h"

// render_element

static inline render_element* initRenderE(void (*rendering_func)(void*),
                                          void* args)
{
  MEMCREATE(render_element*, elem, malloc(sizeof(render_element)));
  elem->rendering_funcs = rendering_func;
  elem->args            = args;
  return elem;
}

void freeRenderE(void* e)
{
  FREE(e);
  return;
}

static void voidFree(void* e)
{
  (void)e;
  return;
}

// Traversal in reversing order is heavy but assuming there is never a lot of
// things to be rendered, this should be fast enough.
void render(renderable* renderable)
{
  if (!renderable) return;
  struct HASH_NODE* node = renderable->start;
  render_element* elem;
  list* l = initList(sizeof(void*));
  while (node)
  {
    insertElement(l, &(node->element));
    node = node->next;
  }
  for (int i = l->size - 1; i >= 0; i--)
  {
    elem = *((render_element**)RZBL_L_ELEM(l, i));
    elem->rendering_funcs(elem->args);
  }
  freeList(l, voidFree);
  return;
}

renderable* initRenderable(void)
{
  renderable* map = hashmapInit();
  return map;
}

void updateArgument(renderable* renderable, int UUID, void* newarg)
{
  if (!renderable) return;
  void* render_e = hashmapFind(renderable, (int)UUID);
  if (!render_e) return;
  ((render_element*)render_e)->args = newarg;
  return;
}

int renderableAdd(renderable* renderable, void (*rendering_func)(void*),
                  void* args)
{
  render_element* elem = initRenderE(rendering_func, args);
  int uuid             = random();
  hashmapInsert(renderable, uuid, elem);
  return uuid;
}

void renderableRemove(renderable* renderable, int UUID)
{
  if (!renderable) return;
  hashmapDelete(renderable, UUID, freeRenderE);
  return;
}

void freeRenderable(renderable* renderable)
{
  if (!renderable) return;
  hashmapFree(renderable, freeRenderE);
}
