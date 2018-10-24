#ifndef READDATA_H
#define READDATA_H

#include "DLList.h"
#include "Graph.h"
#include "BSTree.h"

DLList List_of_Urls();

Graph GetGraph(DLList List_of_Urls);

BSTree GetInvertedList(DLList L);
#endif
