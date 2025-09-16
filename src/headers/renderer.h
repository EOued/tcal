#ifndef RENDERER_H
#define RENDERER_H

#include "macro.h"
#include "stdio.h"

typedef struct
{
  void** args;
  void (**rendering_funcs)(void*);
  unsigned int _capacity;
  unsigned int _size;
  unsigned int* _emptyelements_indexes;
  unsigned int _emptyelements_capacity;
  unsigned int _emptyelements_size;

} renderable;

// Render all renderable functions
void render(renderable* renderable);
// Init renderable
renderable* initRenderable(void);

void freeRenderable(renderable* renderable);
// Add a renderable function to the renderable, return an UUID for said function
// in the renderable
int renderableAdd(renderable* renderable, void (*rendering_func)(void*),
                  void* args);

void renderableRemove(renderable* renderable, unsigned int UUID);
#endif
