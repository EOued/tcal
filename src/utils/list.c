#include "list.h"
#include "macro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// For complexity computations, it will be assumed that a call to an allocation
// function (malloc, calloc, etc) is done in constant time.

// o(1)
list* initList(size_t size)
{
  list* l;
  MEMCHK(l = calloc(1, sizeof(list)));
  l->size     = 0;
  l->capacity = 2;
  MEMCHK(l->elements = calloc(2, size));
  l->esize = size;
  return l;
}

// o(l->size + k), with k the complexity of freeFunc (constant most of the time,
// giving o(l->size) complexity)
void freeList(list* l, void (*freeFunc)(void*))
{
  for (unsigned int i = 0; i < l->size; i++) freeFunc(RZBL_L_ELEM(l, i));
  FREE(l->elements);
  FREE(l);
  return;
}

// o(l->esize) (amortized complexity).
void insertElement(list* l, void* elem)
{
  REALLOC(l->size, l->capacity, l->esize, l->elements);
  memcpy(RZBL_L_ELEM(l, l->size++), elem, l->esize);
  return;
}

// o(1)
void* pop(list* l)
{
  if (l->size <= 0) return NULL;
  return RZBL_L_ELEM(l, --l->size);
}

// o(l->esize)
void* popPos(list* l, unsigned int pos)
{
  if (!l && l->size <= pos) return NULL;
  void* temp;
  MEMCHK(temp = calloc(1, l->esize));
  memcpy(temp, RZBL_L_ELEM(l, pos), l->esize);
  memcpy(RZBL_L_ELEM(l, pos), RZBL_L_ELEM(l, l->size - 1), l->esize);
  memcpy(RZBL_L_ELEM(l, l->size - 1), temp, l->esize);
  FREE(temp);
  return pop(l);
}

// o(l->size * k), with o(k) the complexity of match_func.
int list_pmatch(list* l, int (*match_func)(void*, void*), void* params)
{
  if (!l) return -1;
  for (unsigned int i = 0; i < l->size; i++)
    if (match_func(RZBL_L_ELEM(l, i), params)) return i;
  return -1;
}
