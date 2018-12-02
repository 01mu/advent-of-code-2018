/*
 * hash table implementation
 * github.com/01mu
 */

#include "hash.h"

void init_map(struct Map * map, int size)
{
    map->count = 0;
    map->size = size;
    map->values = malloc(sizeof(struct MapItem *) * size);

    for(int i = 0; i < size; i++)
    {
        map->values[i] = NULL;
    }
}

int hash_code(int key, int size)
{
    if(key < 0)
    {
        return -(key % size);
    }

    return key % size;
}

struct MapItem * search(int key, struct Map * map)
{
    int idx = hash_code(key, map->size);

    struct MapItem * list = map->values[idx];
    struct MapItem * temp = list;

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

void insert(int key, int data, struct Map * map)
{
    int idx = hash_code(key, map->size);
    struct MapItem * list = map->values[idx];

    struct MapItem * temp = list;

    while(temp)
    {
        if(temp->key == key)
        {
            temp->data = data;

            return;
        }

        temp = temp->next;
    }

    struct MapItem * newNode = malloc(sizeof(struct MapItem));

    newNode->key = key;
    newNode->data = data;
    newNode->next = list;

    map->values[idx] = newNode;

    map->count++;
}

struct MapItem * delete(struct MapItem * item, struct Map * map)
{
    int key = item->key;
    int idx = hash_code(key, map->size);

    struct MapItem * temp;

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
