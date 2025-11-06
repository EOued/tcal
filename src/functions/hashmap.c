#include "functions.h"
#include "macro.h"

// fhashmap

fhashmap* fhashmapInit(void)
{
  MEMCREATE(fhashmap*, map, calloc(1, sizeof(fhashmap)));
  map->start = NULL;
  return map;
}

void fhashmapFree(fhashmap* map)
{
  if (!map) return;
  struct fHASH_NODE* node = map->start;
  struct fHASH_NODE* temp;
  while (node)
  {
    temp = node->next;
    FREE(node);
    node = temp;
  }
  FREE(map);
  return;
}

void fhashmapInsert(fhashmap* map, int key, int (*element)(void*))
{
  if (!map) return;
  MEMCREATE(struct fHASH_NODE*, node, calloc(1, sizeof(struct fHASH_NODE)));
  node->key     = key;
  node->element = element;
  node->next    = map->start;
  map->start    = node;
  return;
}
int (*fhashmapFind(fhashmap* map, int key))(void*)
{
  if (!map) return NULL;
  struct fHASH_NODE* node = map->start;
  while (node)
  {
    if (node->key == key) return node->element;
    node = node->next;
  }
  return NULL;
}

// hashmap

hashmap* hashmapInit(void)
{
  MEMCREATE(hashmap*, map, calloc(1, sizeof(hashmap)));
  map->start = NULL;
  return map;
}

void hashmapFree(hashmap* map, void (*freeFunc)(void*))
{
  if (!map) return;
  struct HASH_NODE* node = map->start;
  struct HASH_NODE* temp;
  while (node)
  {
    temp = node->next;
    freeFunc(node->element);
    FREE(node);
    node = temp;
  }
  FREE(map);
  return;
}

void hashmapInsert(hashmap* map, int key, void* element)
{
  if (!map) return;
  MEMCREATE(struct HASH_NODE*, node, calloc(1, sizeof(struct HASH_NODE)));
  node->key     = key;
  node->element = element;
  node->next    = map->start;
  map->start    = node;
  return;
}
void* hashmapFind(hashmap* map, int key)
{
  if (!map) return NULL;
  struct HASH_NODE* node = map->start;
  while (node)
  {
    if (node->key == key) return node->element;
    node = node->next;
  }
  return NULL;
}
