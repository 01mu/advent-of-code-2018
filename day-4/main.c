/*
 * Advent of Code Day 4 - https://adventofcode.com/2018/day/4
 * github.com/01mu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Time
{
    int sleep;
    int wake;
};

struct A
{
    int total;
    int count;

    struct Time ** bounds;
};

#include "../lib/hash/tm/hash.c"
#include "../lib/hash/int/hash.c"

int cmp(const void * a, const void * b);
char * strstr_new(char * str, int begin);
void tokenize_input(char ** inputs, int input_count, struct Map_t * guards);
void assign(char ** list, struct Map_t * guards);
void get_sleepy(struct Map_t * guards);

int guard_count = 0;
int * guard_ids;

int main()
{
    FILE * fp = fopen("bigboy4", "r");

    char line[255];
    int input_count = 0;
    char ** inputs = malloc(sizeof(char *));

    struct Map_t guards;

    while(fgets(line, 255, fp) != NULL)
    {
        inputs[input_count] = malloc(sizeof(char) * strlen(line));
        strcpy(inputs[input_count], line);
        inputs = realloc(inputs, sizeof(char *) * ++input_count + 1);
    }

    guard_ids = malloc(sizeof(int));
    qsort(inputs, input_count, sizeof(inputs[0]), cmp);
    init_map_t(&guards, input_count);
    tokenize_input(inputs, input_count, &guards);
    get_sleepy(&guards);
}

int cmp(const void * a, const void * b)
{
  return strcmp(* (const char **) a, * (const char **) b );
}

char * strstr_new(char * str, int begin)
{
    int len = strlen(str);
    int i = begin;
    int count = 1;
    char * new = malloc(sizeof(char *) * count + 1);
    int d = 0;

    while(i != len)
    {
        new[d] = str[i];
        new = realloc(new, sizeof(char *) * (count + 1));
        count++;
        i++, d++;
    }

    new[d] = '\0';

    return new;
}

void tokenize_input(char ** inputs, int input_count, struct Map_t * guards)
{
    for(int i = 0; i < input_count; i++)
    {
        int j = 0;

        char * q;
        char * list[4];

        q = strtok(inputs[i], " ");

        while(q != NULL)
        {
            if(j == 4)
            {
                j = 0;
                break;
            }

            list[j++] = q;
            q = strtok(NULL, " ");
        }

        assign(list, guards);
    }
}

void assign(char ** list, struct Map_t * guards)
{
    int last_id;
    int last_fall;
    int woke;

    char * id_str;
    int id;

    struct MapItem_t * tm;
    struct A * a;

    char * time_str;

    struct Time * time;
    size_t sz;

    if(strcmp(list[2], "Guard") == 0)
    {
        id_str = strstr_new(list[3], 1);
        id = atoi(id_str);

        tm = search_t(id, guards);

        if(!tm)
        {
            guard_ids[guard_count] = id;
            guard_ids = realloc(guard_ids, sizeof(int) * ++guard_count + 1);

            a = malloc(sizeof(struct A));
            a->count = 0;
            a->total = 0;
            a->bounds = malloc(sizeof(struct Time *));

            insert_t(id, a, guards);
        }

        last_id = id;

        free(id_str);
    }
    else if(strcmp(list[2], "falls") == 0)
    {
        time_str = strstr_new(list[1], 3);
        time_str[2] = '\0';
        last_fall = atoi(time_str);
        free(time_str);
    }
    else
    {
        time_str = strstr_new(list[1], 3);
        time_str[2] = '\0';
        woke = atoi(time_str);
        free(time_str);

        tm = search_t(last_id, guards);

        time = malloc(sizeof(struct Time));
        time->sleep = last_fall;
        time->wake = woke;

        tm->data->total += woke - last_fall;
        tm->data->bounds[tm->data->count] = time;

        sz = sizeof(struct Time *) * ++tm->data->count + 1;

        tm->data->bounds = realloc(tm->data->bounds, sz);
    }
}

void get_sleepy(struct Map_t * guards)
{
    int largest_id;
    int largest_sec = 0;
    int winner;

    int largest_id_b;
    int largest_sec_b = 0;
    int winner_b;

    int most_sleepy = 0;
    int most_sleepy_id;

    struct Map seconds;
    struct MapItem_t * tm;

    int total;

    int sleep;
    int wake;

    for(int i = 0; i < guard_count; i++)
    {
        init_map(&seconds, 10000);

        tm = search_t(guard_ids[i], guards);

        total = tm->data->total;

        for(int j = 0; j < tm->data->count; j++)
        {
            sleep = tm->data->bounds[j]->sleep;
            wake = tm->data->bounds[j]->wake;

            for(int z = sleep; z < wake; z++)
            {
                struct MapItem * itm = search(z, &seconds);

                if(itm)
                {
                    itm->data++;

                    if(itm->data > largest_sec && total >= most_sleepy)
                    {
                        most_sleepy = total;
                        largest_sec = itm->data;
                        winner = z;
                        largest_id = guard_ids[i];
                        most_sleepy_id  = guard_ids[i];
                    }

                    if(itm->data > largest_sec_b)
                    {
                        largest_sec_b = itm->data;
                        winner_b = z;
                        largest_id_b = guard_ids[i];
                    }
                }
                else
                {
                    insert(z, 1, &seconds);
                }
            }
        }
    }

    printf("(A): %i\n", largest_id * winner);
    printf("(B): %i\n", largest_id_b * winner_b);
}
