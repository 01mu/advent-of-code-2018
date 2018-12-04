/*
 * hash table implementation
 * github.com/01mu
 */

#ifndef HASH_HT_
#define HASH_HT_

struct Map_t
{
    int total;
    int size;

    int count;

    struct MapItem_t ** values;
    struct MapItem_t * dummy_item;
    struct MapItem_t * item;
};

struct MapItem_t
{
    struct A * data;
    int key;
    struct MapItem_t * next;
};

void init_map_t(struct Map_t * map, int size);
void free_map_t(struct Map_t * map);
int hash_code_t(int key, int size);
struct MapItem_t * search_t(int key, struct Map_t * map);
void insert_t(int key, struct A * data, struct Map_t * map);
struct MapItem_t * delete_t(struct MapItem_t * item, struct Map_t * map);

#endif
