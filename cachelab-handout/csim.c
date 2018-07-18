#include "cachelab.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef struct
{
    int valid;
    int tag;
    int time_counter;
}line;
typedef line *entry_of_lines;
typedef entry_of_lines *entry_of_sets;

typedef struct
{
    int hit;
    int miss;
    int eviction;
}result;

entry_of_sets InitializeCache(int S, int E)
{
    entry_of_sets cache;
    cache = calloc(S, sizeof(entry_of_lines));

    for(int i = 0;i < S; i++)
    {
        cache[i] = calloc(E, sizeof(line));
    }

    return cache;
}

void ReleaseMemory(entry_of_sets cache, int S, int E)
{
    for (int i = 0;i < S; i++)
    {
        free(cache[i]);
    }

    free(cache);
}

result update(entry_of_lines search_line, result Result, int E, int tag, bool verbose)
{
    int oldest_time = INT_MAX;
    int youngest_time = 0;
    int oldest_block = INT_MAX;
    bool hit_flag = false;

    for (int i = 0; i < E; i++)
    {
        if ((search_line[i].tag == tag) && search_line[i].valid)
        {
            if (verbose)
                printf("hit\n");
            hit_flag = true;
            Result.hit++;
            (search_line[i].time_counter)++;
            break;
        }
    }

    if  (!hit_flag)
    {
        if (verbose)
            printf("miss");
        ++Result.miss;
        for (int i = 0;i < E; i++)
        {
            if (search_line[i].time_counter < oldest_time)
            {
                oldest_time = search_line[i].time_counter;
                oldest_block = i;
            }
            if (search_line[i].time_counter > youngest_time)
            {
                youngest_time = search_line[i].time_counter;
            }
        }

        search_line[oldest_block].time_counter = youngest_time + 1;
        search_line[oldest_block].tag = tag;

        if (search_line[oldest_block].valid)
        {
            if (verbose)    printf(" and eviction\n");
            ++Result.eviction;
        }
        else
        {
            if (verbose)    printf("\n");
            search_line[oldest_block].valid = true;
        }
    }
    return Result;
}

result ReadAndTest(FILE* tracefile, entry_of_sets cache,int S,int E,int s,int b,bool verbose)
{
    result Result = {0,0,0};
    char ch;
    unsigned address;
    int temp;

    while((fscanf(tracefile, " %c %x,%d", &ch, &address, &temp)) == 3)
    {
        if (ch == 'I')
        {
            continue;
        }
        else
        {
            int set_index = (address >> b) & ((-1U) >> (32 - s));
            int tag = (address >> (b + s));
            entry_of_lines search_line = cache[set_index];

            if (ch == 'L' || ch == 'S')
            {
                if (verbose)
                    printf("%c %x", ch, address);
                Result = update(search_line, Result, E, tag, verbose);
            }

            else if (ch == 'M')
            {
                if (verbose)
                    printf("%c %x", ch, address);
                Result = update(search_line, Result, E, tag, verbose);
                Result = update(search_line, Result, E, tag, verbose);
            }

            else
                continue;
        }
    }
    return Result;
}



int main(int argc, char* argv[])
{
    result Result = {0,0,0};
    entry_of_sets cache = NULL;
    bool verbose = false;
    char *help_message = "here is some help message";
    int s = 0;
    int S = 0;
    int E = 0;
    int b = 0;
    FILE* tracefile = NULL;

    int ch;
    while((ch = getopt(argc,argv,"hv::s:E:b:t:")) != -1)
    {
        switch(ch)
        {
            case 'h':
            {
                printf("%s\n", help_message);
                break;
            }

            case 'v':
            {
                verbose = true;
                break;
            }

            case 's':
            {
                s = atol(optarg);
                S = 1 << s;
                break;
            }

            case 'E':
            {
                E = atol(optarg);
                break;
            }

            case 'b':
            {
                b = atol(optarg);
                break;
            }

            case 't':
            {
                if ((tracefile = fopen(optarg, "r")) == NULL)
                {
                    perror("damaged files!\n");
                    exit(-1);
                }
                break;
            }

            default:
            {
                printf("default error\n");
                exit(-1);
            }
        }
    }

    cache = InitializeCache(S,E);
    Result = ReadAndTest(tracefile, cache, S, E, s, b, verbose);
    ReleaseMemory(cache, S, E);
    //printf("hits:%d misses:%d evictions:%d\n", Result.hit, Result.miss, Result.eviction);

    printSummary(Result.hit, Result.miss, Result.eviction);
    return 0;
}

