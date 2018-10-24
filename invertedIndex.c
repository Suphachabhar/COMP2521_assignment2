#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "readData.h"
#include "DLList.h"
#include "Graph.h"
#include "BSTree.h"

int main(int argc, char **argv)
{

    DLList L = List_of_Urls();
    BSTree invert_index = GetInvertedList(L);
    //free(invert_index);
    FILE *fp = fopen("invertedIndex.txt", "w+");
    //BSTreeInfix(invert_index);
    BSTreeInfixToFile(invert_index,fp);
    fclose(fp);

    return 0;
    //}
}
