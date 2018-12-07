/*
 * Advent of Code Day 6 - https://adventofcode.com/2018/day/6
 * github.com/01mu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

const int MAX_BOARD_X = 375;
const int MAX_BOARD_Y = 375;

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
int bad_pos(int x, int y, struct Point ** input_points, int input_count);
void get_distances(struct Point ** input_points, int input_count, int t);

int main()
{
    FILE * fp = fopen("input", "r");

    char line[255];
    int input_count = 0;
    struct Point ** points = malloc(sizeof(struct Point *));

    int t = 0;
    int i;
    int j;
    int k;

    int min;
    int min_id;

    while(fgets(line, 255, fp) != NULL)
    {
        points[input_count] = get_point(line, input_count);
        points = realloc(points, (sizeof(struct Point *)) * ++input_count + 1);
    }

    get_distances(points, input_count, 0);

    int cz = points[0]->d_count;

    int totals[cz];
    int duplicates[cz];
    int mins[cz];

    int a[input_count];
    int bounds[input_count];

    for(i = 0; i < cz; i++)
    {
        min = INT_MAX;

        for(j = 0; j < input_count; j++)
        {
            for(k = 0; k < input_count; k++)
            {
                if( points[j]->distances[i]->dist ==
                    points[k]->distances[i]->dist &&
                    points[j]->id != points[k]->id)
                {
                    if(points[j]->distances[i]->dist < min)
                    {
                        min = points[j]->distances[i]->dist;
                    }
                }
            }
        }

        duplicates[i] = min;
    }

    for(i = 0; i < input_count; i++)
    {
        a[i] = 1;
    }

    for(i = 0; i < cz; i++)
    {
        min = INT_MAX;

        for(int j = 0; j < input_count; j++)
        {
            if(points[j]->distances[i]->dist < min)
            {
                min = points[j]->distances[i]->dist;
                min_id = points[j]->id;

                mins[i] = min;

                if( (points[j]->distances[i]->y == MAX_BOARD_Y - 1) ||
                    (points[j]->distances[i]->x == 0) ||
                    (points[j]->distances[i]->y == 0) ||
                    (points[j]->distances[i]->x == MAX_BOARD_X - 1))
                {
                    bounds[min_id] = 1;
                }
            }
        }

        if(min < duplicates[i])
        {
            a[min_id]++;
        }
    }

    for(int i = 0; i < input_count; i++)
    {
        if(bounds[i] != 1 && a[i] > t)
        {
            t = a[i];
        }
    }

    printf("(A): %i\n", t);
    get_distances(points, input_count, 1);

    t = 0;

    for(i = 0; i < cz; i++)
    {
        t = 0;

        for( j = 0; j < input_count; j++)
        {
           t += points[j]->distances[i]->dist;
        }

        totals[i] = t;
    }

    t = 0;

    for(i = 0; i < cz; i++)
    {
        if(totals[i] < 10000)
        {
            t++;
        }
    }

    printf("(B): %i\n", t);
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
                dis[ic]->dist = abs(i - dest->x) + abs(j - dest->y);
                dis = realloc(dis, (sizeof(struct Cord *)) * ++ic + 1);
            }
        }

        dest->d_count = ic;
        dest->distances = dis;
    }
}

