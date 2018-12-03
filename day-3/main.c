/*
 * Advent of Code Day 3 - https://adventofcode.com/2018/day/3
 * github.com/01mu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../lib/hash/int/hash.c"

const int BOARD_MAX_X = 1500;

struct Cut
{
    int id;

    int x_start;
    int y_start;
    int x_dim;
    int y_dim;

    int start;
    int bottom_max;
    int right_max;

    int * occupied_int;

    struct Map * occupied;
    int occupied_count;

    int overlap;
};

struct Cut * get_cut(char * line);
char * strstr_new(char * str, int begin);
void assign(struct Cut ** cuts, int cut_total);
int get_start(int y_start, int x_start);
void find_occupied(struct Cut ** cuts, int cut_total);
void find_overlap(struct Cut ** cuts, int cut_total);
void find_overlap_prev(struct Cut ** cuts, int cut_total, int occ);
void free_cuts(struct Cut ** cuts, int cut_total);

int main()
{
    int i;
    int j;

    FILE * fp = fopen("input", "r");

    char line[255];
    int cut_total = 0;
    struct Cut ** cuts = malloc(sizeof(struct Cut *));

    while(fgets(line, 255, fp) != NULL)
    {
        cuts[cut_total] = get_cut(line);
        cuts = realloc(cuts, sizeof(struct Cut *) * ++cut_total + 1);
    }

    assign(cuts, cut_total);
    find_occupied(cuts, cut_total);
    find_overlap(cuts, cut_total);

    for(i = 0; i < cut_total; i++)
    {
        if(cuts[i]->overlap == 0)
        {
            printf("(B): %i\n", cuts[i]->id);
        }
    }

    free_cuts(cuts, cut_total);
}

void free_cuts(struct Cut ** cuts, int cut_total)
{
    int i;

    for(i = 0; i < cut_total; i++)
    {
        free(cuts[i]);
    }

    free(cuts);
}

void find_overlap_prev(struct Cut ** cuts, int cut_total, int occ)
{
    int i;
    int j;

    for(i = 0; i < cut_total; i++)
    {
        if(search(occ, cuts[i]->occupied))
        {
            cuts[i]->overlap = 1;
        }
    }
}

void find_overlap(struct Cut ** cuts, int cut_total)
{
    int i;
    int j;
    int total = 0;
    int occ;

    struct Map pos;
    struct MapItem * tm;

    init_map(&pos, BOARD_MAX_X);

    for(i = 0; i < cut_total; i++)
    {
        for(j = 0; j < cuts[i]->occupied_count; j++)
        {
            occ = cuts[i]->occupied_int[j];
            tm = search(occ, &pos);

            if(tm)
            {
                if(tm->data == 1)
                {
                    find_overlap_prev(cuts, cut_total, occ);
                    total++;
                }

                tm->data = 2;
            }
            else
            {
                insert(occ, 1, &pos);
            }
        }
    }

    printf("(A): %i\n", total);
}

void find_occupied(struct Cut ** cuts, int cut_total)
{
    int i;
    int bm;
    int j;
    int n = 0;
    int pos;

    int total = 0;
    int oc = 0;
    int * occupied = malloc(sizeof(int));

    for(i = 0; i < cut_total; i++)
    {
        struct Map * map = malloc(sizeof(struct Map));
        init_map(map, cuts[i]->x_dim * cuts[i]->y_dim);

        bm = cuts[i]->bottom_max;

        while(bm >= cuts[i]->start)
        {
            j = bm;

            while(j <= bm + (cuts[i]->x_dim - 1))
            {
                pos = bm + n;
                j++;
                n++;
                total++;

                insert(pos, pos, map);

                occupied[oc++] = pos;
                occupied = realloc(occupied, sizeof(int) * total + 1);
            }

            n = 0;
            bm = bm - BOARD_MAX_X;
        }

        cuts[i]->occupied_count = total;
        cuts[i]->occupied = map;

        cuts[i]->occupied_int = malloc(total * sizeof(int));
        memcpy(cuts[i]->occupied_int, occupied, total * sizeof(int));

        occupied = realloc(occupied, sizeof(int));

        oc = 0;
        total = 0;
    }

    free(occupied);
}

void assign(struct Cut ** cuts, int cut_total)
{
    int i;
    int start;

    for(i = 0; i < cut_total; i++)
    {
        cuts[i]->start = get_start(cuts[i]->y_start, cuts[i]->x_start);

        start = cuts[i]->start;

        cuts[i]->bottom_max = start + (BOARD_MAX_X * (cuts[i]->y_dim - 1));
        cuts[i]->right_max = start + (cuts[i]->x_dim - 1);
        cuts[i]->overlap = 0;
    }
}

int get_start(int y_start, int x_start)
{
    int start;

    if(y_start == 0)
    {
        start = x_start + 1;
    }
    else
    {
        start = BOARD_MAX_X * (y_start) + (x_start + 1);
    }

    return start;
}

struct Cut * get_cut(char * line)
{
    int i = 0;

    struct Cut * cut = malloc(sizeof(struct Cut));

    char * q;
    char * full_split[4];
    char * pos_split[2];
    char * id;

    q = strtok(line, " ");

    while(q != NULL)
    {
        full_split[i++] = q;
        q = strtok(NULL, " ");
    }

    i = 0;

    id = strstr_new(full_split[0], 1);
    cut->id = atoi(id);
    free(id);

    q = strtok(full_split[2], ",");

    while(q != NULL)
    {
        pos_split[i++] = q;
        q = strtok(NULL, ",");
    }

    i = 0;

    pos_split[1][strlen(pos_split[1]) - 1] = '\0';

    cut->x_start = atoi(pos_split[0]);
    cut->y_start = atoi(pos_split[1]);

    q = strtok(full_split[3], "x");

    while(q != NULL)
    {
        pos_split[i++] = q;
        q = strtok(NULL, "x");
    }

    pos_split[1][strlen(pos_split[1]) - 1] = '\0';

    cut->x_dim = atoi(pos_split[0]);
    cut->y_dim = atoi(pos_split[1]);

    return cut;
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
