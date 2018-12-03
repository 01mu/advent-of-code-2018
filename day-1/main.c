/*
 * Advent of Code Day 1 - https://adventofcode.com/2018/day/1
 * github.com/01mu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../lib/hash/int/hash.c"

void find_total(int * changes, int change_total);
void find_double(int * changes, int change_total);

int main()
{
    FILE * fp = fopen("input", "r");

    char line[255];
    int change_total = 1;
    int * changes = malloc(sizeof(int));

    while(fgets(line, 255, fp) != NULL)
    {
        changes[change_total - 1] = atoi(line);
        changes = realloc(changes, sizeof(int) * ++change_total);
    }

    find_total(changes, change_total);
    find_double(changes, change_total);

    free(changes);
}

void find_total(int * changes, int change_total)
{
    int i;

    int freq = 0;

    --change_total;

    for(i = 0; i < change_total; i++)
    {
        freq += changes[i];
    }

    printf("(A): %i\n", freq);
}

void find_double(int * changes, int change_total)
{
    int i;

    int found = 0;
    int freq = 0;

    struct Map freq_sums;

    init_map(&freq_sums, --change_total);

    while(1)
    {
        for(i = 0; i < change_total; i++)
        {
            freq += changes[i];

            if(search(freq, &freq_sums))
            {
                printf("(B): %i\n", freq);
                found = 1;
                break;
            }
            else
            {
                insert(freq, freq, &freq_sums);
            }
        }

        if(found)
        {
            break;
        }
    }
}
