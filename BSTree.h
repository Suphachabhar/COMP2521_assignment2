// BSTree.h ... interface to binary search tree ADT

#ifndef BSTREE_H
#define BSTREE_H


typedef struct BSTNode *BSTLink;

typedef struct SetNode *BSTSet;

typedef struct SetNode {
   char   *url;
   BSTSet next;
} SetNode;


typedef struct BSTNode
{
	char *key;
	BSTSet url_list;
	BSTLink left, right;
} BSTNode;

typedef struct BSTNode *BSTree;

BSTSet insertNode(BSTSet s, char *url);

// create an empty BSTree
BSTree newBSTree();
// free memory associated with BSTree
void dropBSTree(BSTree);
// display a BSTree
void showBSTree(BSTree);
// display BSTree root node
void showBSTreeNode(BSTree);

// print values in infix order
void BSTreeInfix(BSTree);
// print values in prefix order
void BSTreePrefix(BSTree);
// print values in postfix order
void BSTreePostfix(BSTree);


// print values in infix order
void BSTreeInfixToFile(BSTree,FILE*);
// print values in prefix order
void BSTreePrefixToFile(BSTree,FILE*);
// print values in postfix order
void BSTreePostfixToFile(BSTree,FILE*);

// count #nodes in BSTree
int BSTreeNumNodes(BSTree);
// count #leaves in BSTree
int BSTreeNumLeaves(BSTree);

// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree, char*);
// check whether a value is in a BSTree
BSTree BSTreeFind(BSTree, char*);
// delete a value from a BSTree
BSTree BSTreeDelete(BSTree, char*);

#endif
