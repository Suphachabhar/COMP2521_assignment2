#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "readData.h"
#include "DLList.h"
#include "Graph.h"
#include "BSTree.h"
#include "c99.h"

void fatal(char *);
BSTree updateInvertedIndex(BSTree t, FILE *data, char *url);
char *normalized(char *text);

DLList List_of_Urls()
{
    DLList list_of_urls = newDLList();

    FILE *data;
    if ((data = fopen("collection.txt", "r")) == NULL)
    {
        fatal("Couldn't open file: collection.txt");
    }
    int num_of_nodes = 0;
    char url[10];
    while (fscanf(data, "%s", url) == 1)
    {
        DLListAfter(list_of_urls, url);
        num_of_nodes++;
    }
    fclose(data);
    //printDLList(list_of_urls);
    return list_of_urls;
}

BSTree GetInvertedList(DLList L)
{
    BSTree inv_tree = newBSTree();
    DLListNode *curr = L->first;
    while (curr != NULL)
    {
        char *url = curr->value;
        char *url_file = malloc(sizeof(url));
        strcpy(url_file, url);
        strcat(url_file, ".txt");
        //printf("\n========%s=======\n", url_file);
        FILE *data = fopen(url_file, "r");
        inv_tree = updateInvertedIndex(inv_tree, data, curr->value);
        // printf("\n===== %s =====\n", url);
        // BSTreeInfix(inv_tree);
        // printf("\n===== %s =====\n", url);
        fclose(data);

        curr = curr->next;
    }

    return inv_tree;
}

BSTree updateInvertedIndex(BSTree t, FILE *data, char *url)
{
    char text[100];
    int state = 0;
    // printf("\n===== %s =====\n", url);
    while (fscanf(data, "%s", text) == 1)
    {
        switch (state)
        {
        //finding section-1 to start reading
        case 0:
            if (strcmp(text, "Section-2") == 0)
                state = 1;
            break;
        //add edge (url,text)
        case 1:
            if (strcmp(text, "#end") == 0)
            {
                state = 2;
            }
            else
            {
                // printf("%s\n", url);
                char *n_text = normalized(text);
                t = BSTreeInsert(t, n_text);
                BSTree t_node = BSTreeFind(t, n_text);
                t_node->url_list = insertNode(t_node->url_list, url);
                //BSTreeInfix(t);
                //printf("\n");
            }
            break;
        }
    }
    //BSTreeInfix(t);
    return t;
}

char *normalized(char *text)
{
    //int size = strlen(text);
    int i = 0;
    while (text[i] != '\0')
    {
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            int offset = text[i] - 'A';
            text[i] = 'a' + offset;
        }
        if (text[i] == '.' || text[i] == ',' || text[i] == ';' || text[i] == '?')
        {
            text[i] = '\0';
        }
        i++;
    }

    return text;
}

Graph GetGraph(DLList list_of_Urls)
{
    // Create	empty	graph	(use	graph	ADT	in	say	graph.h and	graph.c)
    // For	each	url in	the	above	list
    // • read	<url>.txt	file,	and	update	graph	by	adding	a	node	and
    // outgoing	links
    int n = DLListLength(list_of_Urls);
    Graph g = newGraph(n);
    if (n == 0)
    {
        return g;
    }
    else
    {
        //move curr to first position
        DLListMoveTo(list_of_Urls, 1);
        int i;
        //loop each position to get url and then read file
        for (i = 0; i < n; i++)
        {
            char *url = DLListCurrent(list_of_Urls);
            char *url_file = malloc(sizeof(url));
            strcpy(url_file, url);
            strcat(url_file, ".txt");
            DLListMove(list_of_Urls, 1);
            //printf("=============--> %s\n", url_file);
            FILE *data;
            if ((data = fopen(url_file, "r")) == NULL)
            {
                fatal(strcat(strcat("Couldn't open file: ", url_file), ".txt"));
            }
            char text[100];
            int state = 0;
            while (fscanf(data, "%s", text) == 1)
            {
                switch (state)
                {
                //finding section-1 to start reading
                case 0:
                    if (strcmp(text, "Section-1") == 0)
                        state = 1;
                    break;
                //add edge (url,text)
                case 1:
                    if (strcmp(text, "#end") == 0)
                    {
                        state = 2;
                    }
                    else
                    {
                        if (strcmp(url, text) != 0)
                        {
                            //printf("%s %s\n",url,text);
                            addEdge(g, url, text);
                        }
                    }
                    break;
                }
            }
            fclose(data);
        }
        return g;
    }
}

// print message for unrecoverable error and stop
void fatal(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}
