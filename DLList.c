// DLList.c - Implementation of doubly-linked list ADT
// Written by John Shepherd, March 2013
// Modified by John Shepherd, August 2014, August 2015

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "DLList.h"
#include "c99.h"


// data structures representing DLList

// typedef struct DLListNode DLListNode;

void swap(DLListNode *a,DLListNode *b);

// create a new DLListNode (private function)
static DLListNode *newDLListNode(char *it)
{
 DLListNode *new;
 new = malloc(sizeof(DLListNode));
 assert(new != NULL);
 new->value = strdup(it);
 new->prev = new->next = NULL;
 return new;
}

// create a new DLListNode with page rank and outdegree (private function)
static DLListNode *newPageRankDLListNode(char *it, float pagerank,int outdegree)
{
 DLListNode *new;
 new = malloc(sizeof(DLListNode));
 assert(new != NULL);
 new->value = strdup(it);
 new->prev = new->next = NULL;
 new->pagerank = pagerank;
 new->outdegree = outdegree;
 return new;
}

// create a new empty DLList
DLList newDLList()
{
 struct DLListRep *L;

 L = malloc(sizeof (struct DLListRep));
 assert (L != NULL);
 L->nitems = 0;
 L->first = NULL;
 L->last = NULL;
 L->curr = NULL;
 return L;
}

// free up all space associated with list
void freeDLList(DLList L)
{
 assert(L != NULL);
 DLListNode *curr, *prev;
 curr = L->first;
 while (curr != NULL) {
  prev = curr;
  curr = curr->next;
  free(prev->value);
  free(prev);
 }
 free(L);
}

// trim off \n from strings (private function)
// this is needed for getDLList() because of fgets()
// alternatively, we could use the evil gets() function
static char *trim(char *s)
{
 int end = strlen(s)-1;
 if (s[end] == '\n') s[end] = '\0';
 return s;
}

// create an DLList by reading items from a file
// assume that the file is open for reading
// assume one item per line, line < 999 chars
DLList getDLList(FILE *in)
{
 DLList L;
 DLListNode *new;
 char line[1000];

 L = newDLList();
 while (fgets(line,1000,in) != NULL) {
  char *value = strdup(trim(line));
  new = newDLListNode(value);
  if (L->last == NULL) {
   L->first = L->last = new;
  }
  else {
   L->last->next = new;
   new->prev = L->last;
   L->last = new;
  }
  L->nitems++;
 } 
 L->curr = L->first;
 return L;
}

// display list to file, one item per line
// assumes that the file is open for writing
void putDLList(FILE *out, DLList L)
{
 assert(out != NULL); assert(L != NULL);
 DLListNode *curr;
 for (curr = L->first; curr != NULL; curr = curr->next)
  fprintf(out,"%s, %d, %.7f\n",curr->value,curr->outdegree,curr->pagerank);
}

// display all items in list
void printDLList(DLList L){
    assert(L != NULL);
    DLListNode *curr;
    printf("num of nodes is %d\n",L->nitems);
    int i = 0;
    for (curr = L->first; curr != NULL; curr = curr->next){
        printf("%d : %s out: %d pagerank: %.7f\n",i,curr->value,curr->outdegree,curr->pagerank);
        i++;
    }
}

void printDLListOnlyVal(DLList L){
    assert(L != NULL);
    DLListNode *curr;
    for (curr = L->first; curr != NULL; curr = curr->next){
        printf("%s\n",curr->value);
    }
}

// check sanity of a DLList (for testing)
int validDLList(DLList L)
{
 if (L == NULL) {
  fprintf(stderr,"DLList is null\n");
  return 0;
 }
 if (L->first == NULL) {
  // list is empty; curr and last should be null
  if (L->last != NULL || L->curr != NULL) {
   fprintf(stderr,"Non-null pointers in empty list\n");
   return 0;
  }
 }
 else {
  // list is not empty; curr and last should be non-null
  if (L->last == NULL || L->curr == NULL) {
   fprintf(stderr,"Null pointers in non-empty list\n");
   return 0;
  }
 }
 int count;
 DLListNode *curr;
 // check scanning forward through list
 count = 0;
 for (curr = L->first; curr != NULL; curr = curr->next) {
  if (curr->prev != NULL && curr->prev->next != curr) {
   fprintf(stderr, "Invalid forward link into node (%s)\n",curr->value);
   return 0;
  }
  if (curr->next != NULL && curr->next->prev != curr) {
   fprintf(stderr, "Invalid backward link into node (%s)\n",curr->value);
   return 0;
  }
  count++;
 }
 if (count != L->nitems) {
  fprintf(stderr, "Forward count mismatch; counted=%d, nitems=%d\n",
          count, L->nitems);
  return 0;
 }
 // check scanning backward through list
 count = 0;
 for (curr = L->last; curr != NULL; curr = curr->prev) {
  count++;
 }
 if (count != L->nitems) {
  fprintf(stderr, "Backward count mismatch; counted=%d, nitems=%d\n",
          count, L->nitems);
  return 0;
 }
 // nothing went wrong => must be ok
 return 1;
}

// return item at current position
char *DLListCurrent(DLList L)
{
 assert(L != NULL); assert(L->curr != NULL);
 return L->curr->value;
}

// move current position (+ve forward, -ve backward)
// return 1 if reach end of list during move
// if current is currently null, return 1
int DLListMove(DLList L, int n)
{
 assert(L != NULL);
 if (L->curr == NULL)
  return 1;
 else if (n > 0) {
  while (n > 0 && L->curr->next != NULL) {
   L->curr = L->curr->next;
   n--;
  }
 }
 else if (n < 0) {
  while (n < 0 && L->curr->prev != NULL) {
   L->curr = L->curr->prev;
   n++;
  }
 }
 return (L->curr == L->first || L->curr == L->last) ? 1 : 0;
}

// move to specified position in list
// i'th node, assuming first node has i==1
int DLListMoveTo(DLList L, int i)
{
 assert(L != NULL); assert(i > 0);
 L->curr = L->first;
 return DLListMove(L, i-1);
}

// insert an item before current item
// new item becomes current item
void DLListBefore(DLList L, char *it)
{
 assert(L != NULL); 
 // COMPLETE THIS FUNCTION
    // insert new node before cuurent node
    // new node become current node

    DLListNode *ins = newDLListNode(it);
    // case 1 : nothing in the list
    if(L->nitems == 0){
        L->first = ins;
        L->last = ins;
        ins->prev = NULL;
        ins->next = NULL;
    }
    // case 2 : insert before the first node
    else if(L->curr->prev == NULL){
        L->first = ins;
        //L->curr = ins;
        ins->prev = NULL;
        ins->next = L->curr;
        L->curr->prev = ins;
    
       
    }
    // case 3 : insert somewhere
    else{
        ins->next = L->curr;
        ins->prev = L->curr->prev;
        //L->curr->prev->next = ins;
        L->curr->prev = ins;
    }
    L->curr = ins;
    L->nitems++;

    
}

//insert an item with pagerank and outdegree
void DLListInsertWithPageRank(DLList L, char *it, float pagerank, int outdegree){
    assert(L != NULL); 
    DLListNode *ins = newPageRankDLListNode(it,pagerank,outdegree);
    // case 1 : nothing in the list
    if(L->nitems == 0){
        L->first = ins;
        L->last = ins;
        ins->prev = NULL;
        ins->next = NULL;
    }
    // case 2 : insert after the last node
    else if(L->curr->next == NULL){
        L->last = ins;
        ins->prev = L->curr;
        ins->next = NULL;
        L->curr->next = ins;
    }
    // case 3 : insert somewhere
    else{
        ins->prev = L->curr;
        ins->next = L->curr->next;
        //L->curr->next->prev = ins;
        L->curr->next = ins;
    }
    L->curr = ins;
    L->nitems++;
}

void sortDLListByPageRank(DLList L){
    assert(L != NULL);
    int swapped;
    DLListNode *curr;
    DLListNode *last = NULL;
    do{
        swapped = 0;
        curr = L->first;
        while(curr->next != last){
            if(curr->pagerank < curr->next->pagerank){
                swapped = 1;
                swap(curr,curr->next);
            }
            curr=curr->next;
        }
        last = curr;
    }
    while(swapped);
}

void sortDLListByOutdegreeAndPageRank(DLList L){
    assert(L != NULL);
    int swapped;
    DLListNode *curr;
    DLListNode *last = NULL;
    do{
        swapped = 0;
        curr = L->first;
        while(curr->next != last){
            if(curr->outdegree < curr->next->outdegree){
                swapped = 1;
                swap(curr,curr->next);
            } else if(curr->outdegree == curr->next->outdegree){
                if(curr->pagerank < curr->next->pagerank){
                    swapped = 1;
                    swap(curr,curr->next);
                }
            } else{}
            curr=curr->next;
        }
        last = curr;
    }
    while(swapped);
}

void swap(DLListNode *a,DLListNode *b){
    char * temp = a->value;
    float pagerank = a->pagerank;
    int outdegree = a->outdegree;
    a->value = b->value;
    a->pagerank = b->pagerank;
    a->outdegree = b->outdegree;
    b->value = temp;
    b->pagerank = pagerank;
    b->outdegree = outdegree;
}

// insert an item after current item
// new item becomes current item
void DLListAfter(DLList L, char *it)
{
 assert(L != NULL); 
 // COMPLETE THIS FUNCTION
    // insert after the current node
    // new node becomr the current node
    // insert after the last node, it will become the last node instead

    DLListNode *ins = newDLListNode(it);
    // case 1 : nothing in the list
    if(L->nitems == 0){
        L->first = ins;
        L->last = ins;
        ins->prev = NULL;
        ins->next = NULL;
    }
    // case 2 : insert after the last node
    else if(L->curr->next == NULL){
        L->last = ins;
        ins->prev = L->curr;
        ins->next = NULL;
        L->curr->next = ins;
    }
    // case 3 : insert somewhere
    else{
        ins->prev = L->curr;
        ins->next = L->curr->next;
        //L->curr->next->prev = ins;
        L->curr->next = ins;
    }
    L->curr = ins;
    L->nitems++;

}

// delete current item
// new item becomes item following current
// if current was last, current becomes new last
// if current was only item, current becomes null
void DLListDelete(DLList L)
{
 assert (L != NULL);
 // COMPLETE THIS FUNCTION
    DLListNode *del = L->curr;

    // case 1 : nothing in the list
    // Does it needed?
    // ...
    if(L->nitems == 0){
        return;
    }
    // case 2 : the node removed is the only node in the list
    // the current become NULL
    else if(del->prev == NULL && del->next == NULL){
        //L->first == NULL;
        //L->last == NULL;
        //L->curr == NULL;
        return;
    }
    // case 3 : the node removed is the first node
    else if(del->prev == NULL){
        L->first = L->curr->next;
        //del->next->prev = NULL;
        del->prev = NULL;
        free(del->value);
        free(del);
        L->curr = L->curr->next;
    }
    // case 4 : the node removed is the last node
    else if(del->next == NULL){
        L->last = L->curr->prev;
        //del->prev->next = NULL;
        del->next = NULL;
        free(del->value);
        free(del);
        L->curr = L->curr->prev;
    }
    // case 4 : the node removed is somewhere
    else{
        //del->next->prev = del->prev;
        //del->prev->next = del->next;
        free(del->value);
        free(del);
        L->curr = L->curr->next;
    }
    L->nitems--;
}

// return number of elements in a list
int DLListLength(DLList L)
{
 return (L->nitems);
}

// is the list empty?
int DLListIsEmpty(DLList L)
{
 return (L->nitems == 0);
}