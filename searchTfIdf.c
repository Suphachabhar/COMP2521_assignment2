#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "URLList.h"
#include "c99.h"

#define TRUE 1
#define FALSE 0


int readTermFromURL(char *searchTerm, char *urlArray[BUFSIZ],char *file);
int getNumberOfDocument(char *file);
double tfCalculate(char *term, char *url);
double idfCalculate(double containingDocs, double nDocs);
char *normalize(char *text);
void showURLList(URLList headOfList);
void freeURLList(URLList headOfList);
URLList calculateTFIDF(URLList headOfList, char *searchTerm, int nDocs);


// Make a sorted list of urls based on number of matching search terms and tfidf
int main(int argc, char *argv[]){
    URLList urlList = NULL;
    //get number of URL from collection.txt
    int nDocs = getNumberOfDocument("collection.txt");
    
    // For each URL, add to and update URLList
    int i;
    for(i = 1; i < argc; i++){
        urlList = calculateTFIDF(urlList, argv[i], nDocs);
    }
    
    // Print the results 
    showURLList(urlList);
    
    freeURLList(urlList);
    return 0;
}


URLList calculateTFIDF(URLList headOfList, char *searchTerm, int nDocs){
    char *URLArray[BUFSIZ];
    int nURLs = readTermFromURL(searchTerm, URLArray, "invertedIndex.txt");
    
    if (nURLs == 0) return headOfList;
    
    double tfidf, idf = idfCalculate(nURLs, nDocs);

    URLList url = NULL;
    int i = 0;
    
    if (headOfList == NULL) {
        tfidf = tfCalculate(searchTerm, URLArray[i]) * idf;
        url = newURLNode(URLArray[i], tfidf);
        headOfList = url;
        i++;
    }
    
    while(i < nURLs){
        tfidf = tfCalculate(searchTerm, URLArray[i]) * idf;
        if((url = URLInList(headOfList, URLArray[i])) != NULL){ // Updates existing node position
            url->numOfTerms++;
            url->tfidf += tfidf;
            headOfList = sortList(headOfList, url); // Reposition url based on numOfTerms and tfidf
        }else{    // Adds new URL to list
            url = newURLNode(URLArray[i], tfidf);
            headOfList = insertURL(headOfList, url); // Position url based on numOfTerms and tfidf
        }
        i++;
    }

    return headOfList;
}

// Returns array of URLs matching search term
int readTermFromURL(char *searchTerm, char *urlArray[BUFSIZ],char *file) {
    FILE *invertedIndex = fopen(file, "r");
    char fileLine[BUFSIZ], *searchTermLine, *term;
    int found = FALSE;
    
    // Loops through entire file and finds search term
    while (fgets(fileLine, BUFSIZ, invertedIndex) != NULL){
        searchTermLine = strdup(fileLine);
        term = strsep(&searchTermLine, " ");
        if (strcmp(term, searchTerm) == 0) {    // check if this is the line containing urls for the search term
            strsep(&searchTermLine, " ");    // Skip the second space
            found = TRUE;
            break;
        }
    }
    
    // Search term doesn't appear in collection
    if (found == FALSE)
        return 0;
        
    // Extract each URL matching search term and place into array
    char *token = strsep(&searchTermLine, " ");
    int i = 0;
    while(searchTermLine != '\0'){
        urlArray[i] = malloc(strlen(token) + 1);
        strcpy(urlArray[i], token);
        token = strsep(&searchTermLine, " ");
        i++;
    }
    
    // Extract the last url 
    if (token != NULL) {
        token = strsep(&token, "\n"); // Strip the trailing newline
        urlArray[i] = malloc(strlen(token) + 1);
        strcpy(urlArray[i], token);
        i++;
    }
    
    fclose(invertedIndex);
    return i;
}



// Get the number of documents in collection.txt
int getNumberOfDocument(char *file) {
    FILE *collection = fopen(file, "r"); 
    int nDocs = 0;
    char temp[100];
    while (fscanf(collection, " %s", temp) == 1) {
        nDocs++;
    }
    fclose(collection);
    return nDocs;
}

// Calculate the term frequency of a term within a page, proportionate to the number of words
double tfCalculate(char *term, char *url) {
    // Open the url file
    char *fileName = malloc(sizeof(url));
    strcpy(fileName, url);
    strcat(fileName, ".txt");

    FILE *page = fopen(fileName, "r");

    double count = 0.0, words, termFreq;
    fscanf(page, "%*[^\n]%*[^#]#%*[^#]#%*[^\n]\n"); // Skip to section 2 

    char word[BUFSIZ];
    fscanf(page, " %s", word);
        
    // Loop through words to find term frequency
    for (words = 0.0; strcmp(word, "#end") != 0; words++) {
        // Normalise the word
        strcpy(word,normalize(word));
        
        if (strcmp(term,word) == 0)
            count++;
        fscanf(page, " %s", word);
    }
    
    termFreq = count/words;
    return termFreq;
}

// Calculate inverse document frequency
double idfCalculate(double containingDocs, double nDocs) {
    double quotient = nDocs/containingDocs;
    double idf = log10(quotient);
    return idf;
}

char *normalize(char *text)
{
    //int size = strlen(text);
    int i = 0;
    char *dup_str = strdup(text);
    while (dup_str[i] != '\0')
    {
        if (dup_str[i] >= 'A' && dup_str[i] <= 'Z')
        {
            int offset = dup_str[i] - 'A';
            dup_str[i] = 'a' + offset;
        }
        if (dup_str[i] == '.' || dup_str[i] == ',' || dup_str[i] == ';' || dup_str[i] == '?')
        {
            dup_str[i] = '\0';
        }
        i++;
    }

    return dup_str;
}

void showURLList(URLList headOfList) {
    URLList curr = headOfList;
    int i = 0;
    while(curr != NULL && i < 30) {
        printf("%s  %.6f\n", curr->URL, curr->tfidf);
        curr = curr->next;
        i++;
    }
}

// clean up the urllist
void freeURLList(URLList headOfList) {
    URLList curr = headOfList, next;
    while (curr != NULL) {
        free(curr->URL);
        next = curr->next;
        free(curr);
        curr = next;
    }
}


