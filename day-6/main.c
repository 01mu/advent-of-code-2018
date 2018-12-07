/*
 * Advent of Code Day 6 - https://adventofcode.com/2018/day/6
 * github.com/01mu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_BOARD_X 375
#define MAX_BOARD_Y 375

struct Cord
{
    int x;
    int y;
    int dist;
};

struct Point
{
    int id;
    int d_count;
    int x;
    int y;
    struct Cord ** distances;
};

struct Point * get_point(char * line, int input_count);
int md(int x1, int y1, int x2, int y2);

int bad_pos(int x, int y, struct Point ** input_points, int input_count)
{
    int i;
    int b = 0;
    int bx;
    int by;

    for(i = 0; i < input_count; i++)
    {
        bx = input_points[i]->x;
        by = input_points[i]->y;

        if(bx == x && by == y)
        {
            b = 1;
        }
    }

    return b;
}

void get_distances(struct Point ** input_points, int input_count, int t)
{
    int ic;

    struct Point * dest;
    struct Cord ** dis;

    for(int z = 0; z < input_count; z++)
    {
        ic = 0;
        dest = input_points[z];
        dis = malloc(sizeof(struct Cord *));

        for(int i = 0; i < MAX_BOARD_X; i++)
        {
            for(int j = 0; j < MAX_BOARD_Y; j++)
            {
                if(bad_pos(i, j, input_points, input_count))
                {
                    if(t == 0)
                    {
                        continue;
                    }
                }

                dis[ic] = malloc(sizeof(struct Cord));
                dis[ic]->x = i;
                dis[ic]->y = j;
                dis[ic]->dist = md(i, j, dest->x, dest->y);
                dis = realloc(dis, (sizeof(struct Cord *)) * ++ic + 1);
            }
        }

        dest->d_count = ic;
        dest->distances = dis;
    }
}

int main()
{
    FILE * fp = fopen("input", "r");

    char line[255];
    int input_count = 0;
    struct Point ** points = malloc(sizeof(struct Point *));

    while(fgets(line, 255, fp) != NULL)
    {
        points[input_count] = get_point(line, input_count);
        points = realloc(points, (sizeof(struct Point *)) * ++input_count + 1);
    }

    get_distances(points, input_count, 0);

    int cz = points[0]->d_count;

    int max_dups[cz];

    for(int i = 0; i < cz; i++)
    {
        int max_dup = INT_MAX;

        for(int j = 0; j < input_count; j++)
        {
            for(int k = 0; k < input_count; k++)
            {
                int a_dist = points[j]->distances[i]->dist;
                int b_dist = points[k]->distances[i]->dist;
                int a_id = points[j]->id;
                int b_id = points[k]->id;

                if(a_dist == b_dist && a_id != b_id)
                {
                    if(a_dist < max_dup)
                    {
                        max_dup = a_dist;
                    }
                }
            }
        }

        max_dups[i] = max_dup;
    }

    int mins[cz];

    int a[input_count];
    int bb[input_count];

    for(int i = 0; i < input_count; i++)
    {
        a[i] = 1;
    }

    for(int i = 0; i < cz; i++)
    {
        int min = INT_MAX;
        int min_id;

        for(int j = 0; j < input_count; j++)
        {
            if(points[j]->distances[i]->dist < min)
            {
                min = points[j]->distances[i]->dist;
                min_id = points[j]->id;

                mins[i] = min;

                int x = points[j]->distances[i]->x;
                int y = points[j]->distances[i]->y;

                if( (y == MAX_BOARD_Y - 1) ||
                    (x == 0) || (y == 0) ||
                    (x == MAX_BOARD_X - 1))
                {
                    bb[min_id] = 1;
                }
            }
        }

        if(min < max_dups[i])
        {
            a[min_id]++;
        }
    }

    int max = 0;

    for(int i = 0; i < input_count; i++)
    {
        if(bb[i] != 1 && a[i] > max)
        {
            max = a[i];
        }
    }

    get_distances(points, input_count, 1);

    printf("(A): %i\n", max);

    int totals[cz];

    for(int i = 0; i < cz; i++)
    {
        int total = 0;

        for(int j = 0; j < input_count; j++)
        {
           total += points[j]->distances[i]->dist;
        }

        totals[i] = total;
    }

    int m = 0;

    int thing = 0;

    for(int i = 0; i < cz; i++)
    {
        if(totals[i] < 10000)
        {
            //printf("%i\n", m);
            thing++;
        }
    }

    printf("(B): %i\n", thing);
}

struct Point * get_point(char * line, int input_count)
{
    int i = 0;

    char * q;
    char * co[2];

    struct Point * point = malloc(sizeof(struct Point));

    line[strlen(line) - 1] = '\0';

    q = strtok(line, ", ");

    while(q != NULL)
    {
        co[i++] = q;
        q = strtok(NULL, ", ");
    }

    point->x = atoi(co[0]);
    point->y = atoi(co[1]);
    point->id = input_count;
    point->d_count = 0;

    return point;
}

int md(int x1, int y1, int x2, int y2)
{
     return abs(x1 - x2) + abs(y1 - y2);
}
