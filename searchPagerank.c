#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "set.h"
#include "DLList.h"

#define MAX_RESULTS 30

int isMatch(char *word, char *argv[]);
void GetMatchedURLs(FILE *inv_idx, Set s, char *word);
//int countMatches(char *url, Set s);
float findPageRank(char *url);

int main(int argc, char *argv[]) {

    // Open invertedIndex.txt for reading
    FILE *inv_idx = fopen("invertedIndex.txt", "r");
    char temp[100];

    Set matched_urls = newSet();
    // search for matching words
    while (fscanf(inv_idx, "%s", temp) != EOF) {
        char *word = malloc(strlen(temp) + 1);
        word = strcpy(word, temp);
        //printf("find word: %s\n",word);
        // if found, store urls for that word
        if (isMatch(word, argv)) {
            //printf("Match word!!: %s\n",word);
            GetMatchedURLs(inv_idx, matched_urls, word);
        }
    }
    //showSet(matched_urls);
    fclose(inv_idx);

    if (matched_urls == NULL) {
        return 0;
    }
    DLList ranked_urls = newDLList();
    // for each url
    Node *curr = matched_urls->elems;

    while (curr != NULL) {
        // count number of search terms matched
        
        //int matches = countMatches(curr->val, matched_urls);
        // find page rank for that url
        //printf("==>%s\n",curr->val);
        float page_rank = findPageRank(curr->val);
        DLListInsertWithPageRank(ranked_urls,curr->val,page_rank,curr->num);
        //printDLList(ranked_urls);
        // insert into searcList
        //ranked_urls = listInsert(ranked_urls, curr->val, matches, page_rank);

        curr = curr->next;
    }
    
    sortDLListByOutdegreeAndPageRank(ranked_urls);
    //printDLList(ranked_urls);
    printDLListOnlyVal(ranked_urls);
    return 0;
}

// Determines if a word matches any of the arguments
int isMatch(char *word, char *argv[]) {
    int i = 1;
    while (argv[i] != NULL) {
        if (strcmp(word, argv[i]) == 0) {
            return 1;
        }
        i ++;
    }
    return 0;
}

// *To do - improve the efficiency of this
// store urls for a matched term
void GetMatchedURLs(FILE *inv_idx, Set s, char *word) {
    char line[1000];
    //printf("word: %s\n",word);
    // Read a line and look for the word in the line
    // while (fgets(line, 1000, inv_idx)) {
    //     printf("line word: %s %s\n",line,word);
    //     if (strstr(line, word) != NULL) {
    //         break;
    //     }
    // }
    // *Figure out a way that works for this
    // Read the line for urls
    fgets(line, 1000, inv_idx);
    char *buffer = line;
    //printf("buffer url: %s\n",buffer);
    char temp[100];
    int pos;
    while (sscanf(buffer,"%99s%n", temp, &pos) == 1) {
        char *match_url = malloc(strlen(temp) + 1);
        match_url = strcpy(match_url, temp);
        // Store the urls in the set
        if (strstr(match_url, "url") != NULL) {
            //printf("Insert url: %s\n",match_url);
            insertInto(s, match_url);
        }
        buffer += pos;
    }
}

// counts number of times url appears in set
// int countMatches(char *url, Set s) {
//     int count = 0;
//     Node *curr = s->elems;
//     while (curr != NULL) {
//         if (strcmp(curr->url, url) == 0) {
//             count ++;
//         }
//         curr = curr->next;
//     }

//     return count;
// }

float findPageRank(char *url) {
    FILE *pr_list = fopen("pagerankList.txt", "r");
    char line[1000];
    // Read a line and look for the word in the line
    while (fgets(line, 1000, pr_list)) {
        if (strstr(line, url) != NULL) {
            break;
        }
    }

    char temp[100];
    int outgoing;
    float page_rank;
    //printf("findpagerank  : %s\n",line);
    sscanf(line,"%s %d, %f", temp, &outgoing, &page_rank);

    fclose(pr_list);

    return page_rank;
}