#ifndef RENDERER_H
#define RENDERER_H

#include "macro.h"
#include "stdio.h"

typedef hashmap renderable;

typedef struct
{
  void (*rendering_funcs)(void*);
  void* args;
} render_element;

// Render all renderable functions
void render(renderable* renderable);
// Init renderable
renderable* initRenderable(void);
void updateArgument(renderable* renderable, int UUID, void* newarg);
void freeRenderable(renderable* renderable);
// Add a renderable function to the renderable, return an UUID for said function
// in the renderable
int renderableAdd(renderable* renderable, void (*rendering_func)(void*),
                  void* args);

void renderableRemove(renderable* renderable, int UUID);
#endif
