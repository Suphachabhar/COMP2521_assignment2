// graph.h ... Interface to Graph of strings
// Written by John Shepherd, September 2015

#ifndef GRAPH_H
#define GRAPH_H

#include "DLList.h"

typedef struct GraphRep *Graph;

// Function signatures

Graph newGraph(int);
void  disposeGraph(Graph);
int   addEdge(Graph,char *,char *);
int   nVertices(Graph);
int   isConnected(Graph, char *, char *);
void  showGraph(Graph,int);

// add for assn2
DLList calculatePageRank(Graph g,float d,float diffPR,int maxIterations);
void  calculateWeightPageRank(Graph g);

#endif
