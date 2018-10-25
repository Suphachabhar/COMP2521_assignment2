#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "URLList.h"
#include "c99.h"


URLList newURLNode(char *url, double tfidf);
URLList URLInList(URLList list, char *u);
URLList insertURL(URLList headOfList, URLList url);
URLList sortList(URLList headOfList, URLList url);
URLList deleteURL(URLList headOfList, URLList url);


URLList newURLNode(char *url, double tfidf) {
    URLList new = malloc(sizeof(listNode));
    assert(new != NULL);
    new->URL = strdup(url);
    new->tfidf = tfidf;
    new->numOfTerms = 1;
    new->next = NULL;
    return new;
}

// Returns node if url is in list
URLList URLInList(URLList list, char *u){
    URLList curr = list;
    while(curr != NULL){
        if(strcmp(curr->URL, u) == 0){
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

// Insert a URL into the URLList
URLList insertURL(URLList headOfList, URLList url) {
    URLList curr = headOfList, prev = NULL;
    
    if (headOfList == NULL || url->numOfTerms > headOfList->numOfTerms ||
        (url->numOfTerms == headOfList->numOfTerms && url->tfidf > headOfList->tfidf)) {
        url->next = headOfList;
        return url;
    }
    
    while (curr != NULL && curr->numOfTerms > url->numOfTerms) {
        prev = curr;
        curr = curr->next;
    }

    while (curr != NULL && curr->numOfTerms == url->numOfTerms && curr->tfidf > url->tfidf) {
        prev = curr;
        curr = curr->next;
    }
    
    // Insert the url
    if (curr == NULL) { // Append
        prev->next = url;
    }else{ // Put url before curr
        prev->next = url;
        url->next = curr;
    }
    
    return headOfList;
}

// Deletes old url and inserts new url into correct position
URLList sortList(URLList headOfList, URLList url) {
    headOfList = deleteURL(headOfList, url);
    headOfList = insertURL(headOfList, url);

    return headOfList;
}

// Delete a url from the URLList
URLList deleteURL(URLList headOfList, URLList url){
    if (headOfList == url) {
        return url->next;
    }
    else {
        URLList curr = headOfList;
        while (curr->next != url) {
            curr = curr->next;
        }  
        curr->next = url->next;
    }
    return headOfList;
}