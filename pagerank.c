#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "readData.h"
#include "DLList.h"
#include "Graph.h"

int main(int argc, char **argv)
{

    float d, diffPR;
    int maxIterations;
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s d diffPR maxIterations\n", argv[0]);
        exit(1);
    }
    d = atof(argv[1]);
    diffPR = atof(argv[2]);
    maxIterations = atoi(argv[3]);

    DLList L = List_of_Urls();
    Graph g = GetGraph(L);
    calculateWeightPageRank(g);
    //showGraph(g, 1);
   
    DLList list_Urls_PageRanks = calculatePageRank(g,d,diffPR,maxIterations );
    // printDLList(list_Urls_PageRanks);
    sortDLListByPageRank(list_Urls_PageRanks);
    // printDLList(list_Urls_PageRanks);
    FILE *data;
    data = fopen("pagerankList1.txt", "w+");
    putDLList(data,list_Urls_PageRanks);
    return 0;
    //}
}
