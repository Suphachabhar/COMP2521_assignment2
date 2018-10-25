// graph.c ... Graph of strings (adjacency matrix)
// Written by John Shepherd, September 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include "DLList.h"
#include "c99.h"

#define strEQ(g,t) (strcmp((g),(t)) == 0)

typedef unsigned char Num;

typedef struct GraphRep {
	int   nV;
	int   maxV;
	int   *inbound;
	int   *outbound;
	char  **vertex;
	float   **win;
    float   **wout;
	Num   **edges;
	
} GraphRep;

// Function signatures

Graph newGraph();

void  disposeGraph(Graph);
int   addEdge(Graph,char *,char *);
int   nVertices(Graph);
int   isConnected(Graph, char *, char *);
void  showGraph(Graph,int);

static int vertexID(char *, char **, int);
int addVertex(char *, char **, int);

// add for assn2
DLList calculatePageRank(Graph g,float d,float diffPR,int maxIterations);
void  calculateWeightPageRank(Graph g);
float f_abs(float f);

// newGraph()
// - create an initially empty Graph
Graph newGraph(int maxV)
{
	Graph new = malloc(sizeof(GraphRep));
	assert(new != NULL);
	int i, j;
	new->nV = 0;
	new->maxV = maxV;
	new->vertex = malloc(maxV*sizeof(char *));
	assert(new->vertex != NULL);
	new->edges = malloc(maxV*sizeof(Num *));
	new->inbound = malloc(maxV*sizeof(int));
	new->outbound = malloc(maxV*sizeof(int));
	new->win = malloc(maxV * sizeof(float *));
   	new->wout = malloc(maxV * sizeof(float *));
	assert(new->edges != NULL);
	for (i = 0; i < maxV; i++) {
		new->inbound[i] = 0;
		new->outbound[i] = 0;
		new->vertex[i] = NULL;
		new->edges[i] = malloc(maxV*sizeof(Num));
		new->win[i] = malloc(maxV * sizeof(float));
        new->wout[i] = malloc(maxV * sizeof(float));
		assert(new->edges[i] != NULL);
		for (j = 0; j < maxV; j++){
			new->edges[i][j] = 0;
			new->win[i][j] = 0;
			new->wout[i][j] = 0;
		}
			
	}
	return new;
}

float f_abs(float f){
	if(f<0){
		//printf("f_abs: %.7f\n",(-1) * f);
		return (-1) * f;
	}
	//printf("f_abs: %.7f\n",f);
	return f;
	
}

DLList calculatePageRank(Graph g,float d,float diffPR,int maxIterations){
	DLList L = newDLList();
    //printf("%f %f %d\n",d,diffPR,maxIterations);
	int n = g->nV;
	float pagerank[n];
	int i,j;
	for(i=0;i<n;i++){
		pagerank[i] = 1.0/n;
	}
	int iteration = 0;
	float diff = diffPR;
	float damping_factor = (1.0-d)/n;
	while(iteration < maxIterations && diff >= diffPR){
		//printf("iteration: %d\n",iteration);
		float sum_diff = 0;
		for(i=0;i<n;i++){
			float sum_pagerank = 0;
			float pagerank_temp;
			for(j=0;j<n;j++){
				if(g->edges[j][i]){
					sum_pagerank = sum_pagerank+(pagerank[j] * (g->win[j][i]) * (g->wout[j][i]));
				}
					
			}
			pagerank_temp = pagerank[i];
			pagerank[i] = damping_factor + (d*sum_pagerank);
			if(pagerank[i]>pagerank_temp){
				sum_diff = sum_diff+(pagerank[i]-pagerank_temp);
			}else{
				sum_diff = sum_diff+(pagerank_temp-pagerank[i]);
			}
			//sum_diff = sum_diff+f_abs(pagerank_temp-pagerank[i]);
		}
		diff = sum_diff;
		iteration++;

	}
	for(i=0;i<n;i++){
		// printf("pagerank:%s %.7f\n",g->vertex[i],pagerank[i]);
		DLListInsertWithPageRank(L,g->vertex[i],pagerank[i],g->outbound[i]);
	}
	return L;
}

void calculateWeightPageRank(Graph g)
{
	int i,j;
	for(i=0;i<g->nV;i++){
		float in_all = 0;
		float out_all = 0;
		for(j=0;j<g->nV;j++){
			if(g->edges[i][j]){
				in_all = in_all + g->inbound[j];
				//from assignment note
				if (g->outbound[j]==0){
					out_all = out_all + 0.5;
				} else {
					out_all = out_all + g->outbound[j];
				}
				
			}
		}
		//printf("%d %.2f %.2f\n",i,in_all,out_all);
		
		for(j=0;j<g->nV;j++){
			if(g->edges[i][j]){
				//printf("%d %d\n",i,j);
				g->win[i][j] = g->inbound[j]/in_all;
				if (g->outbound[j]==0){
					g->wout[i][j] = 0.5/out_all;
				} else {
					g->wout[i][j] = g->outbound[j]/out_all;
				}
			}
		}
	}
}

// disposeGraph(Graph)
// - clean up memory associated with Graph
void disposeGraph(Graph g)
{
	if (g == NULL) return;
	int i;
	for (i = 0; i < g->nV; i++) {
		free(g->vertex[i]);
	}
	for (i = 0; i < g->maxV; i++) {
		free(g->edges[i]);
		
	}
	free(g->inbound);
	free(g->outbound);
	free(g->edges);
}

// addEdge(Graph,Src,Dest)
// - add an edge from Src to Dest
// - returns 1 if edge successfully added
// - returns 0 if unable to add edge
//   (usually because nV exceeds maxV)
int addEdge(Graph g, char *src, char *dest)
{
	assert(g != NULL);
	int v = vertexID(src,g->vertex,g->nV);
	if (v < 0) {
		if (g->nV >= g->maxV) return 0;
		v = addVertex(src,g->vertex,g->nV);
		g->nV++;
	}
	int w = vertexID(dest,g->vertex,g->nV);
	if (w < 0) {
		if (g->nV >= g->maxV) return 0;
		w = addVertex(dest,g->vertex,g->nV);
		g->nV++;
	}
	g->outbound[v]++;
	g->inbound[w]++;
	g->edges[v][w] = 1;
	return 1;
}

// isConnected(Graph,Src,Dest)
// - check whether there is an edge from Src->Dest
int isConnected(Graph g, char *src, char *dest)
{
	assert(g != NULL);
	int v = vertexID(src,g->vertex,g->nV);
	int w = vertexID(dest,g->vertex,g->nV);
	if (v < 0 || w < 0)
		return 0;
	else
		return g->edges[v][w];
}

// nVertices(Graph)
// - return # vertices currently in Graph
int nVertices(Graph g)
{
	assert(g != NULL);
	return (g->nV);
}

// showGraph(Graph)
// - display Graph
void showGraph(Graph g, int mode)
{
	assert(g != NULL);
	if (g->nV == 0)
		printf("Graph is empty\n");
	else {
		printf("Graph has %d vertices:\n",g->nV);
		int i, j;
		if (mode == 1) {
			for (i = 0; i < g->nV; i++) {
				for (j = 0; j < g->nV; j++)
					printf("%d",g->edges[i][j]);
				putchar('\n');
			}
		}
		else {
			for (i = 0; i < g->nV; i++) {
				printf("Vertex: %s\n", g->vertex[i]);
				printf("connects to\n");
				for (j = 0; j < g->nV; j++) {
					if (g->edges[i][j])
						printf("   %s\n",g->vertex[j]);
				}
			}
		}
		for (i = 0; i < g->nV; i++) {
			printf("nodeid: %d , indegree: %d , outdegree: %d\n",i,g->inbound[i],g->outbound[i]);
		}
		for(i=0;i<g->nV;i++){
			for(j=0;j<g->nV;j++){
				if(g->edges[i][j]){
					printf("Win[%d][%d]: %.7f , Wout[%d][%d]: %.7f\n",i,j,g->win[i][j],i,j,g->wout[i][j]);
				}
			}
		}
	}
}

// Helper functions

// vertexID(Str,Names,N)
// - searches for Str in array of Names[N]
// - returns index of Str if found, -1 if not
static int vertexID(char *str, char **names, int N)
{
	int i;
	for (i = 0; i < N; i++)
		if (strEQ(str,names[i])) return i;
	return -1;
}

// addVertex(Str,Names,N)
// - add Str at end of Names
int addVertex(char *str, char **names, int N)
{
	names[N] = strdup(str);
	return N;
}
