// BSTree.c ... implementation of binary search tree ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "BSTree.h"



// make a new node containing a value
static BSTLink newBSTNode(char *v)
{
	BSTLink new = malloc(sizeof(BSTNode));
	assert(new != NULL);
	new->key = strdup(v);
	new->url_list = NULL;
	new->left = new->right = NULL;
	return new;
}

// create a new empty BSTree
BSTree newBSTree()
{
	return NULL;
}

// free memory associated with BSTree
void dropBSTree(BSTree t)
{
	if (t == NULL)
		return;
	dropBSTree(t->left);
	dropBSTree(t->right);
	free(t);
}

void writeBSTreeNodeInFile(BSTree t,FILE *data)
{
	//printf("writeBSTreeNodeInFile\n");
	if (t == NULL){
		
		return;
	}
	fprintf(data, "%s  ", t->key);

	BSTSet curr = t->url_list;
	while (curr != NULL) {
		fprintf(data, "%s ", curr->url);
		curr = curr->next;
	}
	fprintf(data, "\n");
}

// display BSTree root node
void showBSTreeNode(BSTree t)
{
	if (t == NULL)
		return;
	printf("%s ", t->key);
}

// print values in infix order
void BSTreeInfixToFile(BSTree t,FILE *data)
{
	if (t == NULL)
		return;
	BSTreeInfixToFile(t->left,data);
	writeBSTreeNodeInFile(t,data);
	BSTreeInfixToFile(t->right,data);
}

// print values in prefix order
void BSTreePrefixToFile(BSTree t,FILE *data)
{
	if (t == NULL)
		return;
	writeBSTreeNodeInFile(t,data);
	BSTreePrefixToFile(t->left,data);
	BSTreePrefixToFile(t->right,data);
}

// print values in postfix order
void BSTreePostfixToFile(BSTree t,FILE *data)
{
	if (t == NULL)
		return;
	BSTreePostfixToFile(t->left,data);
	BSTreePostfixToFile(t->right,data);
	writeBSTreeNodeInFile(t,data);
}

// print values in infix order
void BSTreeInfix(BSTree t)
{
	if (t == NULL) return;
	BSTreeInfix(t->left);
	showBSTreeNode(t);
	BSTreeInfix(t->right);
}

// print values in prefix order
void BSTreePrefix(BSTree t)
{
	if (t == NULL) return;
	showBSTreeNode(t);
	BSTreePrefix(t->left);
	BSTreePrefix(t->right);
}

// print values in postfix order
void BSTreePostfix(BSTree t)
{
	if (t == NULL) return;
	BSTreePostfix(t->left);
	BSTreePostfix(t->right);
	showBSTreeNode(t);
}

// count #nodes in BSTree
int BSTreeNumNodes(BSTree t)
{
	if (t == NULL)
		return 0;
	else
		return 1 + BSTreeNumNodes(t->left) + BSTreeNumNodes(t->right);
}

// count #leaves in BSTree
int BSTreeNumLeaves(BSTree t)
{
	if (t == NULL)
	{
		/* here is not exist a node */
		return 0;
	}
	if (t->left == NULL && t->right == NULL)
	{
		/* here is a leave */
		return 1;
	}
	// recursive calculate the leaves
	return BSTreeNumLeaves(t->left) + BSTreeNumLeaves(t->right);
}

// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree t, char *v)
{
	// if (t != NULL){
	//	printf("\nInsertion: %s %s %d\n",v,t->key,strcmp(v, t->key));
	// }
	if (t == NULL){
		//printf("\nInsert NULL\n");
		return newBSTNode(v);
	}
	else if (strcmp(v, t->key) < 0){
		//printf("\nInsert Left\n");
		t->left = BSTreeInsert(t->left, v);
	}
	else if (strcmp(v, t->key) > 0){
		//printf("\nInsert Right\n");
		t->right = BSTreeInsert(t->right, v);
	}
	else{
		//printf("\n Else \n");
	} // (v == t->value)
	
		/* don't insert duplicates */;
	return t;
}

// check whether a value is in a BSTree
BSTree BSTreeFind(BSTree t, char *v)
{
	if (t == NULL)
		return 0;
	else if (strcmp(v, t->key) < 0)
		return BSTreeFind(t->left, v);
	else if (strcmp(v, t->key) > 0)
		return BSTreeFind(t->right, v);
	else // (v == t->value)
		return t;
}

// delete root of tree
static BSTree deleteRoot(BSTree t)
{
	// if no subtrees, tree empty after delete
	if (t->left == NULL && t->right == NULL)
	{
		free(t);
		return NULL;
	}
	// if only right subtree, make it the new root
	else if (t->left == NULL && t->right != NULL)
	{
		free(t);
		return t->right;
	}
	// if only left subtree, make it the new root
	else if (t->left != NULL && t->right == NULL)
	{
		free(t);
		return t->left;
	}
	// else (t->left != NULL && t->right != NULL)
	// so has two subtrees
	// - find inorder successor
	// - move its value to root
	// - delete inorder successor node
	BSTLink parent = t;
	BSTLink succ = t->right; // not null!
	while (succ->left != NULL)
	{
		parent = succ;
		succ = succ->left;
	}
	t->key = succ->key;
	free(succ);
	if (parent == t)
		parent->right = NULL;
	else
		parent->left = NULL;
	return t;
}

// delete a value from a BSTree
BSTree BSTreeDelete(BSTree t, char *v)
{
	if (t == NULL)
		return NULL;
	else if (strcmp(v, t->key) < 0)
		t->left = BSTreeDelete(t->left, v);
	else if (strcmp(v, t->key) > 0)
		t->right = BSTreeDelete(t->right, v);
	else // (v == t->value)
		t = deleteRoot(t);
	return t;
}

static BSTSet newNode(char *url)
{
	BSTSet new = malloc(sizeof(SetNode));
	assert(new != NULL);
	new->url = strdup(url);
	new->next = NULL;
	return new;
}

BSTSet insertNode(BSTSet s, char *url)
{
	if (s == NULL)
	{
		return newNode(url);
	}
	if (strcmp(url, s->url) < 0)
	{
		BSTSet n = newNode(url);
		n->next = s;
		return n;
	}
	else if (strcmp(url, s->url) > 0)
	{
		s->next = insertNode(s->next, url);
	}
	else
	{
	}
	//Avoids duplicates - don't need to check
	return s;
}

BSTSet deleteNode(BSTSet s, char *url)
{
	if (s == NULL)
	{
		return s;
	}
	if (strcmp(s->url, url) == 0)
	{
		SetNode *temp = s->next;
		free(s);
		return temp;
	}
	s->next = deleteNode(s->next, url);
	return s;
}

void freeSet(BSTSet S)
{
	if (S != NULL)
	{
		freeSet(S->next);
		free(S);
	}
}