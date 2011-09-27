/*************************************************************/
/**                                                         **/
/**                 Microsoft RPC Examples                  **/
/**                 Dictionary Application                  **/
/**             Copyright(c) Microsoft Corp. 1992-1995      **/
/**                                                         **/
/*************************************************************/

/**************************************************************/
/*  user interface header file for top down splay package     */
/*  based on Sleator & Tarjan's Self Adjusting Trees          */
/**************************************************************/

typedef int (*cmp_rec_func)(void *, void *);
                            /* type of a comparison function */

typedef struct tnode {
    struct tnode *left;     /* left child pointer */
    struct tnode *right;    /* right child pointer */
    void *item;             /* pointer to some structure */
} TreeNode;

typedef struct dictnode {
    TreeNode *root;                 /* pointer to the root of a SAT     */
    long size;                      /* number of records in dictionary  */
    void * state;                   /* reserved for state info          */
    cmp_rec_func cmp_rec;           /* pointer to a comparison function */
    TreeNode* (*splay)(TreeNode *, void *, cmp_rec_func);
                                    /* pointer to a splay function      */
    void (*print_rec)(void *);      /* one line print function          */
} Dictionary;

/*************************************************************************/
/*****              Core functions (internal)                        *****/
/*************************************************************************/

TreeNode *              /* returns the new root                 */
tdSplay(                /* general top down splay               */
    TreeNode *root,     /* the current root of the tree         */
    void *keyItem,      /* pointer to a "key item" searched for */
    int (*cmp)(void *, void *) );
                        /* pointer to a comparison function     */

TreeNode*
tdSplayLeft(TreeNode* root);

TreeNode*
tdSplayRight(TreeNode* root);

void
print_tree_sat(         /* prints the binary tree (indented right subtree,
                           followed by the root, followed by the indented
                           right dubtree) */
    Dictionary * dp,
    int indent);        /* number of blanks to indent subtrees */

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

typedef enum {
    SUCCESS,
    ITEM_ALREADY_PRESENT,
    ITEM_NOT_FOUND,
    FIRST_ITEM,
    LAST_ITEM,
    EMPTY_DICTIONARY,
    NULL_ITEM
} Dict_Status;

#define DICT_CURR_ITEM(pDict)  ( (pDict)->root->item )

#define DICT_EMPTY(pDict)      ( (pDict)->root == NULL )

Dictionary*
Dict_New(                    /* returns a new dictionary node          */
    int (*cmp_rec)           /* pointer to item comparison function    */
        (void *, void *),
    TreeNode* (*splay)       /* pointer to a splay function            */
        (TreeNode *, void *, cmp_rec_func),

    void (*print_rec)        /* pointer to one line item print routine */
        (void *) );

Dict_Status
Dict_Find(
    Dictionary* dp,     /* Dictionary to be searched. */
    void* item);        /* Item searched for. */

Dict_Status
Dict_Next(
    Dictionary* dp,     /* Dictionary to be searched. */
    void* item);        /* A Key item.  Advance to successor of item in dp. */

Dict_Status
Dict_Prev(
    Dictionary* dp,     /* Dictionary to be searched. */
    void* item);        /* A Key item.  Retreat to predecessor of item in dp. */

Dict_Status
Dict_Insert(            /* insert the given item into the tree */
    Dictionary* dp,     /* the modified dictionary */
    void* item);        /* the item to be inserted */

Dict_Status
Dict_Delete(            /* delete the given item into from the tree */
    Dictionary* dp,     /* the modified dictionary */
    void** pItem);      /* IN: points to the (key) item to be deleted */
                        /* OUT: points to the item just deleted */
