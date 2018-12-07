/*
 * Advent of Code Day 7 - https://adventofcode.com/2018/day/7
 * github.com/01mu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

struct Letter
{
    int is_finished;
    char letter;

    char * reqs;
    int reqs_count;

    char * children;
    int ch_count;
};

#include "../lib/hash/letter/hash.c"

void set_map(char * line, struct Map * letters, char * first, int lc);
char find_min(char * available, int ac, struct Map * letters);
void update_avail(char * available, int * ac, char * reqs, int cc);
int min_req_fin(struct MapItem * itm, struct Map * letters);
void remove_avail(char * available, int * ac, char remove);

int main()
{
    FILE * fp = fopen("input", "r");

    char line[255];
    int letter_count = 0;

    char first;

    char * available = malloc(sizeof(char));
    int available_count = 0;

    struct Map letters;

    init_map(&letters, 100);

    while(fgets(line, 255, fp) != NULL)
    {
        set_map(line, &letters, &first, letter_count);
        ++letter_count;
    }

    for(int i = 65; i < 91; i++)
    {
        struct MapItem * a = search(i, &letters);

        if(a && a->data->reqs_count == 0)
        {
            available[available_count] = i;
            ++available_count;
            available = realloc(available, sizeof(char) * available_count);
        }
    }

    struct MapItem * i;
    char min;

    int test = 0;

    printf("(A): ");

    while(available_count > 0)
    {
        min = find_min(available, available_count, &letters);

        i = search(min, &letters);

        i->data->is_finished = 1;

        remove_avail(available, &available_count, min);
        update_avail(available, &available_count,
            i->data->children, i->data->ch_count);
    }

    printf("\n");
}

void remove_avail(char * available, int * ac, char remove)
{
    for(int i = 0; i < *ac; i++)
    {
        if(available[i] == remove)
        {
            for(int j = i; j < *ac; j++)
            {
                available[j] = available[j + 1];
            }

            putchar(remove);

            *ac = *ac - 1;
            available = realloc(available, sizeof(char) * *ac);
        }
    }
}

void update_avail(char * available, int * ac, char * children, int cc)
{
    int found;

    for(int i = 0; i < cc; i++)
    {
        found = 0;

        for(int j = 0; j < *ac; j++)
        {
            if(available[j] == children[i])
            {
                found = 1;
            }
        }

        if(!found)
        {
            available[*ac] = children[i];
            *ac = *ac + 1;
            available = realloc(available, sizeof(char) * *ac);
        }
    }
}

int min_req_fin(struct MapItem * itm, struct Map * letters)
{
    int i;
    int bad = 0;

    for(i = 0; i < itm->data->reqs_count; i++)
    {
        struct MapItem * t = search(itm->data->reqs[i], letters);

        if(t->data->is_finished == 0)
        {
            bad = 1;
        }
    }

    return bad;
}

char find_min(char * available, int ac, struct Map * letters)
{
    int i;

    struct MapItem * itm;
    char min = '~';

    for(i = 0; i < ac; i++)
    {
        itm = search(available[i], letters);

        if(available[i] < min && !min_req_fin(itm, letters))
        {
            min = available[i];
        }
    }

    return min;
}

void set_map(char * line, struct Map * letters, char * first, int lc)
{
    struct MapItem * itm;
    struct Letter * letter;

    char * q;
    char * get[2];

    int i = 0;
    int n = 0;

    int nc;

    q = strtok(line, " ");

    while(q != NULL)
    {
        if(i == 1 || i ==7)
        {
            get[n] = q;
            n++;
        }

        q = strtok(NULL, " ");
        i++;
    }

    itm = search(get[1][0], letters);

    if(itm)
    {
        itm->data->reqs[itm->data->reqs_count] = get[0][0];
        nc = itm->data->reqs_count++ + 1;
        itm->data->reqs = realloc(itm->data->reqs, sizeof(char) * nc);
    }
    else
    {
        letter = malloc(sizeof(struct Letter));

        letter->is_finished = 0;
        letter->letter = get[1][0];

        letter->reqs = malloc(sizeof(char) + 1);
        letter->children = malloc(sizeof(char) + 1);

        letter->reqs[0] = get[0][0];

        letter->reqs_count = 1;
        letter->ch_count = 0;

        insert(get[1][0], letter, letters);
    }

    itm = search(get[0][0], letters);

    if(itm)
    {
        itm->data->children[itm->data->ch_count] = get[1][0];
        nc = itm->data->ch_count++ + 1;
        itm->data->children = realloc(itm->data->children, sizeof(char) * nc);
    }
    else
    {
        letter = malloc(sizeof(struct Letter));

        letter->is_finished = 0;
        letter->letter = get[0][0];

        letter->reqs = malloc(sizeof(char) + 1);
        letter->children = malloc(sizeof(char) + 1);

        letter->children[0] = get[1][0];

        letter->reqs_count = 0;
        letter->ch_count = 1;

        insert(get[0][0], letter, letters);
    }

    if(lc == 0)
    {
        letter = malloc(sizeof(struct Letter));

        letter->is_finished = 1;
        letter->letter = get[0][0];

        letter->reqs = malloc(sizeof(char) + 1);
        letter->children = malloc(sizeof(char) + 1);

        letter->children[0] = get[1][0];

        letter->reqs_count = 0;
        letter->ch_count = 1;

        insert(get[0][0], letter, letters);

        *first = get[0][0];
    }
}
