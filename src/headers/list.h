#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

// Resizable list
typedef struct
{
  unsigned int size;        // Number of elements
  unsigned int capacity;    // Number of elements that can be contained
  void* elements;           // List of elements. Size is capacity * esize
  size_t esize;             // Size of an element
} list;

// Init an empty list with capacity 2. size is the size (number of bytes) of an element.
list* initList(size_t size);
// Free a list. freeFunc is called on every argument of the list, and should be a free function for said elements.
void freeList(list* l, void (*freeFunc)(void*));
// Insert an element at the end of the list.
void insertElement(list* l, void* elem);
// Delete and return the last element of the list.
void* pop(list* l);
// Delete and return the element at position pos of the list.
void* popPos(list* l, unsigned int pos);
// Return the first position for which match_func returns true. The first parameter of the list is an argument of the list, the second is params. This function is greedy and returns immediatly when match_func returns true.
int list_pmatch(list* l, int (*match_func)(void*, void*), void* params);

#endif
