/*
 * Advent of Code Day 2 - https://adventofcode.com/2018/day/2
 * github.com/01mu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void find_prod(char ** strings, int str_total);
void find_diff(char ** strings, int str_total);
void free_strings(char ** strings, int str_total);
int count_str(char ch, char * str);

int main()
{
    FILE * fp = fopen("input", "r");

    char line[255];
    int str_total = 1;
    char ** strings = malloc(sizeof(char *));

    while(fgets(line, 255, fp) != NULL)
    {
        strings[str_total - 1] = malloc(sizeof(char) * strlen(line) - 1);
        line[strlen(line) - 1] = '\0';
        strcpy(strings[str_total - 1], line);
        strings = realloc(strings, sizeof(char *) * ++str_total);
    }

    --str_total;

    find_prod(strings, str_total);
    find_diff(strings, str_total);

    free_strings(strings, str_total);
}

void find_prod(char ** strings, int str_total)
{
    int i;
    int j;

    int total_2 = 0;
    int total_3 = 0;

    bool has_2;
    bool has_3;

    int count;

    for(i = 0; i < str_total; i++)
    {
        for(j = 0; j < strlen(strings[i]); j++)
        {
            count = count_str(strings[i][j], strings[i]);

            if(count == 2)
            {
                has_2 = true;
            }

            if(count == 3)
            {
                has_3 = true;
            }
        }

        if(has_2)
        {
            total_2++;
        }

        if(has_3)
        {
            total_3++;
        }

        has_2 = false;
        has_3 = false;
    }

    printf("(A): %i\n", total_2 * total_3);
}

void find_diff(char ** strings, int str_total)
{
    int i;
    int j;
    int k;

    int count;
    int str_len = strlen(strings[0]);
    int bad_pos;

    for(i = 0; i < str_total; i++)
    {
        for(j = 0; j < str_total; j++)
        {
            count = 0;

            for(k = 0; k < str_len; k++)
            {
                if(strings[i][k] == strings[j][k])
                {
                    count++;
                }
                else
                {
                    bad_pos = k;
                }
            }

            if(count == str_len - 1)
            {
                printf("(B): %s, %i\n", strings[i], bad_pos);
            }
        }
    }
}

void free_strings(char ** strings, int str_total)
{
    int i;

    for(i = 0; i < str_total + 1; i++)
    {
        free(strings[i]);
    }

    free(strings);
}

int count_str(char ch, char * str)
{
    int count = 0;

    for(int i = 0; i < strlen(str); i++)
    {
        if(str[i] == ch)
        {
            count++;
        }
    }

    return count;
}
