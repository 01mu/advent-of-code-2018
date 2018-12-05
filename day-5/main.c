/*
 * Advent of Code Day 5 - https://adventofcode.com/2018/day/5
 * github.com/01mu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int find_shortest(char * input);
char * rem_ch(char rem, char * input);

#define INPUT #include input

int main()
{
    FILE * fp = fopen("input", "r");

    int i;
    int min = 1000000;
    int res;

    char * input_copy;
    char * new_rem;

    char line[70000];
    char * input = malloc(sizeof(char) * 70000);

    while(fgets(line, 70000, fp) != NULL)
    {
        input = line;
    }

    input[strlen(input) - 1] = '\0';

    input_copy = malloc(sizeof(char) * strlen(input) + 1);

    printf("(A): %i\n", find_shortest(input));

    strcpy(input_copy, input);

    for(i = 97; i < 123; i++)
    {
        strcpy(input_copy, input);
        res = find_shortest(rem_ch(i, input_copy));

        if(res < min)
        {
            min = res;
        }
    }

    printf("(B): %i\n", min);
}

int find_shortest(char * input)
{
    int i = 0;
    int z = 0;
    int tog = 0;

    char ch;

    while(input[i] != '\0')
    {
        ch = input[i];

        if(input[i + 1] == ch - 32 || input[i + 1] == ch + 32)
        {
            z = i;

            while(input[z] != '\0')
            {
                input[z] = input[z + 2];
                z++;
            }

            z = 0;

            tog = 1;
        }

        if(!tog)
        {
            i++;
        }
        else
        {
            if(i != 0)
            {
                i--;
            }

            tog = 0;
        }
    }

    return strlen(input);
}

char * rem_ch(char rem, char * input)
{
    int i = 0;
    int z = 0;
    int tog = 0;

    char ch;

    while(input[i] != '\0')
    {
        ch = input[i];

        if(ch == rem || ch == (rem - 32))
        {
            z = i;

            while(input[z] != '\0')
            {
                input[z] = input[z + 1];
                z++;
            }

            tog = 1;
        }

        if(!tog)
        {
            i++;
        }
        else
        {
            if(i != 0)
            {
                i--;
            }

            tog = 0;
        }
    }

    return input;
}
