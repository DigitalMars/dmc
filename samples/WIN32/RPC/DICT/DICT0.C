/*************************************************************/
/**                                                         **/
/**                 Microsoft RPC Examples                  **/
/**                 Dictionary Application                  **/
/**             Copyright(c) Microsoft Corp. 1992-1995      **/
/**                                                         **/
/*************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <rpc.h>
#include <rpcndr.h>
#include "dict0.h"

/************************************************************************/
TreeNode Dumbo;
// volatile
TreeNode *Dummy = &Dumbo;   // a global dummy node

#define ROTATELEFT  tmp=t->right; t->right=tmp->left; tmp->left=t; t=tmp
#define ROTATERIGHT tmp=t->left; t->left=tmp->right; tmp->right=t; t=tmp
#define LINKLEFT    tmp=t; t=t->right; l=l->right=tmp
#define LINKRIGHT   tmp=t; t=t->left; r=r->left=tmp
#define ASSEMBLE    r->left=t->right; l->right=t->left; \
                    t->left=Dummy->right; t->right=Dummy->left
/************************************************************************/

/************************************************************************
 Basic structure declarations from dict0.h:

typedef struct tnode {
    struct tnode *left;             // left child pointer
    struct tnode *right;            // right child pointer
    void *item;                     // pointer to some structure
} TreeNode;

typedef struct dictnode {
    TreeNode *root;                 // pointer to the root of a SAT
    long size;                      // number of records in dictionary
    void * state;                   // reserved for state info
    cmp_rec_func cmp_rec;           // pointer to a comparison function
    TreeNode* (*splay)(TreeNode *, void *, cmp_rec_func);
                                    // pointer to a splay function
    void (*print_rec)(void *);      // one line print function
} Dictionary;

#define DICT_CURR_ITEM(pDict)       pDict->root->item

typedef enum {
    SUCCESS,
    ITEM_ALREADY_PRESENT,
    ITEM_NOT_FOUND,
    FIRST_ITEM,
    LAST_ITEM,
    EMPTY_DICTIONARY,
    NULL_ITEM
} Dict_Status;
**************************************************************************/


/*************************************************************************/
/***    Minimal Dictionary Operations:                                 ***/
/***                                                                   ***/
/***    Dictionary *Dict_New(Cmp_rec*, Splay*, print_rec*)             ***/
/***                                                                   ***/
/***    Dict_Status Dict_Find(Dictionary*, Item*)                      ***/
/***    Dict_Status Dict_Next(Dictionary*, Item*)                      ***/
/***    Dict_Status Dict_Prev(Dictionary*, Item*)                      ***/
/***    Dict_Status Dict_Insert(Dictionary*, Item*)                    ***/
/***    Dict_Status Dict_Delete(Dictionary*, Item**)                   ***/
/***                                                                   ***/
/***    Item* DICT_CURR_ITEM(Dict*)                                    ***/
/*************************************************************************/

#define TreeNode_New(pnode, pitem) pnode = \
    (TreeNode*) MIDL_user_allocate (sizeof(TreeNode)); \
    pnode->left = pnode->right = NULL; pnode->item = pitem;

Dictionary*
Dict_New(                    // returns a new dictionary node
    int (*cmp_rec)           // pointer to item comparison function
        (void *, void *),
    TreeNode* (*splay)       // pointer to a splay function
        (TreeNode *, void *, cmp_rec_func),
    void (*print_rec)        // pointer to one line item print routine
        (void *) )
{
    Dictionary* dp;

    dp = (Dictionary*) MIDL_user_allocate(sizeof(Dictionary));
    dp->root = NULL;
    dp->size = 0;
    dp->state = NULL;
    dp->print_rec = print_rec;
    dp->splay = splay;
    dp->cmp_rec = cmp_rec;
    return(dp);
}

Dict_Status
Dict_Find(
    Dictionary* dp,     // Dictionary to be searched.
    void* item)         // Item searched for.
{
    int keycmp;
    TreeNode* t;

    if (dp->root == NULL)
        return (EMPTY_DICTIONARY);
    if (item == NULL)
        return(NULL_ITEM);
    t = dp->root = dp->splay(dp->root, item, dp->cmp_rec);
    keycmp = (dp->cmp_rec)( t->item, item );
    if (keycmp != 0)
        return(ITEM_NOT_FOUND);
    else return(SUCCESS);
}

Dict_Status
Dict_Next(
    Dictionary* dp,     // Dictionary to be searched.
    void* item)         // A Key item.  Advance to successor of item in dp.
{
    TreeNode* t;
    int keycmp;

    if (dp->root == NULL)
        return (EMPTY_DICTIONARY);
    if (item == NULL) {
        dp->root = tdSplayLeft(dp->root);
        return(SUCCESS);
    }
    if (item == DICT_CURR_ITEM(dp)) {
        t = dp->root;
        keycmp = 0;
    }
    else {
        dp->root = t = dp->splay(dp->root, item, dp->cmp_rec);
        keycmp = (dp->cmp_rec) (item, t->item);
    }
    if (keycmp < 0)
        return(SUCCESS);
    else if (t->right == NULL) {
        return(LAST_ITEM);
    }
    else {
        t = dp->root;
        dp->root = tdSplayLeft(t->right);
        dp->root->left = t;
        t->right = NULL;
        return(SUCCESS);
    }
}

Dict_Status
Dict_Prev(
    Dictionary* dp,     // Dictionary to be searched.
    void* item)         // A Key item.  Retreat to predecessor of item in dp.
{
    TreeNode* t;
    int keycmp;

    if (dp->root == NULL)
        return (EMPTY_DICTIONARY);
    if (item == NULL) {
        dp->root = tdSplayRight(dp->root);
        return(SUCCESS);
    }
    if (item == DICT_CURR_ITEM(dp)) {
        t = dp->root;
        keycmp = 0;
    }
    else {
        dp->root = t = dp->splay(dp->root, item, dp->cmp_rec);
        keycmp = (dp->cmp_rec) (item, t->item);
    }
    if (keycmp > 0)
        return(SUCCESS);
    else if (t->left == NULL) {
        return(FIRST_ITEM);
    }
    else {
        t = dp->root;
        dp->root = tdSplayRight(t->left);
        dp->root->right = t;
        t->left = NULL;
        return(SUCCESS);
    }
}

Dict_Status
Dict_Insert(            // insert the given item into the tree
    Dictionary* dp,     // the modified dictionary
    void* item)         // the item to be inserted
{
    int keycmp;
    TreeNode *t, *newNode;

    if (item == NULL)
        return(NULL_ITEM);
    if (dp->root == NULL) {
        TreeNode_New(newNode, item);
        dp->root = newNode;
        dp->size++;
        return(SUCCESS);
    }
    t = dp->root = dp->splay(dp->root, item, dp->cmp_rec);
    keycmp = (dp->cmp_rec)( t->item, item );
    if (keycmp == 0)
        return(ITEM_ALREADY_PRESENT);

    TreeNode_New(newNode, item);
    if (keycmp < 0) {   // t->item < item
        newNode->right = t->right;
        t->right = NULL;
        newNode->left = t;
    }
    else {
        newNode->left = t->left;
        t->left = NULL;
        newNode->right = t;
    }
    dp->root = newNode;
    dp->size++;
    return(SUCCESS);
}


Dict_Status
Dict_Delete(            // delete the given item into from the tree
    Dictionary* dp,     // the modified dictionary
    void** pItem)       // IN: points to the (key) item to be deleted
                        // OUT: points to the item just deleted
{
    TreeNode *t, *r;
    int keycmp;
    void* item = *pItem;
    t = dp->root;

    if (item == NULL)
        return(NULL_ITEM);
    if (dp->root == NULL)
        return (EMPTY_DICTIONARY);
    if (item == DICT_CURR_ITEM(dp))
        keycmp = 0;
    else {
        dp->root = t = dp->splay(dp->root, item, dp->cmp_rec);
        keycmp = (dp->cmp_rec)( item, t->item );
    }
    if (keycmp != 0)
        return(ITEM_NOT_FOUND);
    *pItem = DICT_CURR_ITEM(dp);

    if (t->left == NULL) {
        dp->root = t->right;
    }
    else if ((r = t->right) == NULL) {
        dp->root = t->left;
    }
    else  {
        r = tdSplayLeft(r);
        // at this point r->left == NULL
        r->left = t->left;
        dp->root = r;
    }
    t->left = t->right = t->item = NULL;
    MIDL_user_free(t);
    dp->size--;
    return(SUCCESS);
}


/*************************************************************************/
/*****              Core functions (internal)                        *****/
/*************************************************************************/

TreeNode *              // returns the new root
tdSplay(                // general top down splay
    TreeNode *root,     // the current root of the tree
    void *keyItem,      // pointer to a "key item" searched for
    int (*cmp)(void *, void *) )
                        // pointer to a comparison function
{
    TreeNode* t=root;   // current search point
    TreeNode* l;        // root of "left subtree" < keyItem
    TreeNode* r;        // root of "right subtree" > keyItem
    int kcmpt, kcmpleft, kcmpright;
                        // cash comparison results
    TreeNode* tmp;

/***/
    Dummy = &Dumbo;
    l = Dummy;
    r = Dummy;
/***/

    if ( (root == NULL) || ((*cmp)(keyItem, root->item) == 0) )
        return(root);
    Dummy->left = Dummy->right = NULL;
    while ( (kcmpt = (*cmp)(keyItem, t->item)) != 0 ) {
        if ( kcmpt < 0 ) {
            if ( t->left == NULL )
                break;
            if ( (kcmpleft = (*cmp)(keyItem, t->left->item)) == 0 ) {
                LINKRIGHT;
            }
            else if ( kcmpleft < 0 ) {
                ROTATERIGHT;
                if ( t->left != NULL ) {
                    LINKRIGHT;
                }
            }
            else { // keyItem > t->left->item
                LINKRIGHT;
                if ( t->right != NULL ) {
                    LINKLEFT;
                }
            }
        }
        else { // keyItem > t->item
            if ( t->right == NULL )
                break;
            if ( (kcmpright = (*cmp)(keyItem, t->right->item)) == 0 ) {
                LINKLEFT;
            }
            else if ( kcmpright > 0 ) {
                ROTATELEFT;
                if ( t->right != NULL ) {
                    LINKLEFT;
                }
            }
            else { // keyItem < t->right->item
                LINKLEFT;
                if ( t->left != NULL ) {
                    LINKRIGHT;
                }
            }
        }
    }

    ASSEMBLE;
    return(t);
}

TreeNode*
tdSplayLeft(TreeNode* root)
{
    TreeNode* t=root;   // current "search" point
    TreeNode* l=Dummy;  // root of "left subtree" < keyItem
    TreeNode* r=Dummy;  // root of "right subtree" > keyItem
    TreeNode* tmp;

    if ( (t == NULL) || (t->left == NULL) )
        return(t);
    if ( t->left->left == NULL ) {
        ROTATERIGHT;
        return(t);
    }
    Dummy->left = Dummy->right = NULL;
    while ( t->left != NULL ) {
        ROTATERIGHT;
        if ( t->left != NULL ) {
            LINKRIGHT;
        }
    }

    ASSEMBLE;
    return(t);
}

TreeNode*
tdSplayRight(TreeNode* root)
{
    TreeNode* t=root;   // current "search" point
    TreeNode* l=Dummy;  // root of "left subtree" < keyItem
    TreeNode* r=Dummy;  // root of "right subtree" > keyItem
    TreeNode* tmp;

    if ( (t == NULL) || (t->right == NULL) )
        return(t);
    Dummy->left = Dummy->right = NULL;
    while ( t->right != NULL ) {
        ROTATELEFT;
        if ( t->right != NULL ) {
            LINKLEFT;
        }
    }

    ASSEMBLE;
    return(t);
}
