/* hasPath.c 
   Written by Ashesh Mahidadia, October 2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Graph.h"

/* 
    You will submit only this one file.

    Implement the function "hasPath" below. Read the exam paper for 
    detailed specification and description of your task.  

    - You can add helper functions in this file, if required.  

    - DO NOT modify code in the file BSTree.h . 
    - DO NOT add "main" function in this file. 
*/

int *visited; // array of visited

int dfsPathCheck(Graph g, Vertex src, Vertex dest)
{
    int i;
    for (i = 0; i < g->nV; i++)
    {
        if (visited[i] == -1 && g->edges[src][i] > 0)
        {
            visited[i] = src;
            if (i == dest)
            {
                return 1;
            }
            else if (dfsPathCheck(g, i, dest))
            {
                return 1;
            }
        }
    }
    return 0;
}

int hasPath(Graph g, Vertex src, Vertex dest)
{
    return 0;
    assert(g != NULL);
    int i;
    for (i = 0; i < g->nV; i++)
    {
        visited[i] = -1;
    }
    visited[src] = src;
    if (dfsPathCheck(g, src, dest))
    {
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}