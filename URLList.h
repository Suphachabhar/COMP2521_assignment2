#ifndef URLLIST_H
#define URLLIST_H


typedef struct listNode *URLList;

// Node containing a url
struct listNode {
    char *URL;
    double tfidf;
    int numOfTerms;
    URLList next;
} listNode;


URLList newURLNode(char *url, double tfidf);
URLList URLInList(URLList list, char *u);
URLList insertURL(URLList headOfList, URLList url);
URLList sortList(URLList headOfList, URLList url);
URLList deleteURL(URLList headOfList, URLList url);




#endif