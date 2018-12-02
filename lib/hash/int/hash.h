/*
 * hash table implementation
 * github.com/01mu
 */

#ifndef HASH_H_
#define HASH_H_

struct Map
{
    int total;
    int size;

    int count;

    struct MapItem ** values;
    struct MapItem * dummy_item;
    struct MapItem * item;
};

struct MapItem
{
    int data;
    int key;
    struct MapItem * next;
};

void init_map(struct Map * map, int size);
void free_map(struct Map * map);
int hash_code(int key, int size);
struct MapItem * search(int key, struct Map * map);
void insert(int key, int data, struct Map * map);
struct MapItem * delete(struct MapItem * item, struct Map * map);

#endif
