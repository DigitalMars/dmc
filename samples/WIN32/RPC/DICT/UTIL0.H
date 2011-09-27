/*************************************************************/
/**                                                         **/
/**                 Microsoft RPC Examples                  **/
/**                 Dictionary Application                  **/
/**             Copyright(c) Microsoft Corp. 1992-1995      **/
/**                                                         **/
/*************************************************************/

#include <rpc.h>

/*************************************************************************/
/***                RecordNode / RecordTree free routines              ***/
/*************************************************************************/

void
RecordTreeNodeFree(
    IN RecordTreeNode * node
    );

void
RecordTreeFree(
    IN RecordTreeNode * node
    );

VDict_Status
RDict_Free_Dict(
    IN OUT RDict * r_dict
    );

void
VDict_Print(
    VDict * pvd,
    int indent
    );

/*************************************************************************/
/***                  State Allocate / Free routines                   ***/
/*************************************************************************/

DictState * allocate_state(void);

void free_state(DictState * state);

/*************************************************************************/
/***                     Rdict Duplicate utilities                     ***/
/*************************************************************************/

RDict *
RDict_Duplicate(
    IN RDict * src
    );

DictState *
DictState_Duplicate(
    IN DictState * src
    );

TreeNode *
TreeNode_Duplicate(
    IN TreeNode * src
    );

TreeNode *
Tree_Duplicate(
    IN TreeNode * src
    );

/*************************************************************************/
/***                MIDL_user_allocate / MIDL_user_free                ***/
/*************************************************************************/

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t count);

void __RPC_USER MIDL_user_free(void __RPC_FAR * p);

/*************************************************************************/
/***                    Comparison and Printing routines               ***/
/*************************************************************************/

Record *
makeRecord(
    short key,
    char * name
    );

void
freeRecord(
    Record * pr
    );

Record *
ItemDuplicate(
    Record * item
    );

void
ItemCopy(
    IN Record * src,
    OUT Record * dest
    );


int comp(void* x, void* y);

void printRecord(void* rp);

typedef void (*PrintFun) (void *);

void prinTree(
    int lmargin,
    int indent,
    TreeNode *np,
    PrintFun print);

void
Dict_Print(             /* prints the binary tree (indented right subtree,
                           followed by the root, followed by the indented
                           right dubtree) */
    Dictionary * dp,
    int indent);        /* number of spaces to indent subsequent levels */

TreeNode*
makeNode(void * item);

void
Init_dict(Dictionary * dp);
