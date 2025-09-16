#include "renderer.h"
#include "stdlib.h"

void render(renderable* renderable)
{
  if (!renderable) return;
  for (unsigned int i = 0; i < renderable->_size; i++)
    if (renderable->rendering_funcs[i])
      renderable->rendering_funcs[i](renderable->args[i]);
  return;
}

renderable* initRenderable(void)
{
  renderable* r;
  MEMCHK((r = malloc(sizeof(renderable))));
  r->_capacity = 1;
  r->_size     = 1;
  MEMCHK(r->args = calloc(1, sizeof(void*)));
  MEMCHK(r->rendering_funcs = calloc(1, sizeof(void (*)(void*))));
  r->_emptyelements_capacity = 1;
  r->_emptyelements_size     = 0;
  MEMCHK(r->_emptyelements_indexes = calloc(1, sizeof(unsigned int)));
  return r;
}

void updateArgument(renderable* renderable, unsigned int UUID, void* newarg)
{
  if (!renderable || !renderable->args) return;
  renderable->args[UUID] = newarg;
  return;
}

void emptyElementsAddIndex(renderable* renderable, unsigned int index)
{
  if (!renderable || !renderable->_emptyelements_indexes) return;
  REALLOC(renderable->_emptyelements_size, renderable->_emptyelements_capacity,
          sizeof(unsigned int), renderable->_emptyelements_indexes);
  renderable->_emptyelements_indexes[renderable->_emptyelements_size++] = index;
  return;
}

int emptyElementsPop(renderable* renderable)
{
  if (!renderable || !renderable->_emptyelements_indexes ||
      renderable->_emptyelements_size == 0)
    return -1;
  return renderable->_emptyelements_indexes[--renderable->_emptyelements_size];
}

void freeRenderable(renderable* renderable)
{
  if (!renderable) return;
  free(renderable->args);
  free(renderable->rendering_funcs);
  free(renderable->_emptyelements_indexes);
  free(renderable);
  return;
}

int renderableAdd(renderable* renderable, void (*rendering_func)(void*),
                  void* args)
{
  if (!renderable) return -1;
  int emptyElement;
  if ((emptyElement = emptyElementsPop(renderable)) != -1)
  {
    renderable->rendering_funcs[emptyElement] = rendering_func;
    renderable->args[emptyElement]            = args;
    if (renderable->_size <= (unsigned int)emptyElement)
      renderable->_size = emptyElement + 1;
    return emptyElement;
  }
  for (unsigned int index = 0; index < renderable->_size; index++)
  {
    // Hit empty function, we'll put our function here.
    if (!renderable->rendering_funcs[index])
    {
      renderable->rendering_funcs[index] = rendering_func;
      renderable->args[index]            = args;
      return index;
    }
  }

  // Hit end of iteration, list full. We reallocate memory
  int index    = renderable->_size;
  int size     = renderable->_size;
  int capacity = renderable->_capacity;
  REALLOC(renderable->_size++, renderable->_capacity, sizeof(void (*)(void*)),
          renderable->rendering_funcs);
  REALLOC(size, capacity, sizeof(void*), renderable->args);

  renderable->rendering_funcs[index] = rendering_func;
  renderable->args[index]            = args;
  return index;
}

void renderableRemove(renderable* renderable, unsigned int UUID)
{
  if (!renderable || (unsigned int)renderable->_size <= UUID) return;
  emptyElementsAddIndex(renderable, UUID);
  renderable->rendering_funcs[UUID] = NULL;
  renderable->_size--;
  return;
}
