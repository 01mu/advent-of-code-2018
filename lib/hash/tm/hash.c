/*
 * hash table implementation
 * github.com/01mu
 */

#include "hash.h"

void init_map_t(struct Map_t * map, int size)
{
    map->count = 0;
    map->size = size;
    map->values = malloc(sizeof(struct MapItem_t *) * size);

    for(int i = 0; i < size; i++)
    {
        map->values[i] = NULL;
    }
}

int hash_code_t(int key, int size)
{
    if(key < 0)
    {
        return -(key % size);
    }

    return key % size;
}

struct MapItem_t * search_t(int key, struct Map_t * map)
{
    int idx = hash_code_t(key, map->size);

    struct MapItem_t * list = map->values[idx];
    struct MapItem_t * temp = list;

    while(temp)
    {
        if(temp->key == key)
        {
            return temp;
        }

        temp = temp->next;
    }

    return NULL;
}

void insert_t(int key, struct A * data, struct Map_t * map)
{
    int idx = hash_code_t(key, map->size);
    struct MapItem_t * list = map->values[idx];

    struct MapItem_t * temp = list;

    while(temp)
    {
        if(temp->key == key)
        {
            temp->data = data;

            return;
        }

        temp = temp->next;
    }

    struct MapItem_t * newNode = malloc(sizeof(struct MapItem_t));

    newNode->key = key;
    newNode->data = data;
    newNode->next = list;

    map->values[idx] = newNode;

    map->count++;
}

struct MapItem_t * delete_t(struct MapItem_t * item, struct Map_t * map)
{
    int key = item->key;
    int idx = hash_code_t(key, map->size);

    struct MapItem_t * temp;

    while(map->values[idx] != NULL)
    {
      if(map->values[idx]->key == key)
      {
            temp = map->values[idx];

            map->values[idx] = map->dummy_item;

            return temp;
        }

        ++idx;
        idx %= map->size;
    }

    return NULL;
}
