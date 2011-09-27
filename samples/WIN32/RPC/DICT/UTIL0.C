/*************************************************************/
/**                                                         **/
/**                 Microsoft RPC Examples                  **/
/**                 Dictionary Application                  **/
/**          Copyright(c) Microsoft Corp. 1992-1995         **/
/**                                                         **/
/*************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef _LOCAL
#include "replay.h"
#else
#include "play.h"
#endif // _LOCAL

#include "dict0.h"
#include "util0.h"


/*************************************************************************/
/***                RecordNode / RecordTree free routines              ***/
/*************************************************************************/

void
RecordTreeNodeFree(
    IN RecordTreeNode * node
    )
{
    if (node == NULL)
        return;

    MIDL_user_free( node->item->name );
    MIDL_user_free( node->item );
    node->left = NULL;
    node->right = NULL;
    MIDL_user_free( node );
}

void
RecordTreeFree(
    IN RecordTreeNode * node
    )
{
    if (node == NULL)
        return;

    if (node->left != NULL) {
        RecordTreeFree(node->left);
    }
    if (node->right != NULL) {
        RecordTreeFree(node->right);
    }
    RecordTreeNodeFree( node );
}

VDict_Status
RDict_Free_Dict(
    IN OUT RDict * r_dict
    )
{
    RecordTreeFree( r_dict->root );

    return(DICT_SUCCESS);
}

/*************************************************************************/
/***                  State Allocate / Free routines                   ***/
/*************************************************************************/

DictState * allocate_state(void)
{
    DictState * pstate = (DictState*) MIDL_user_allocate(sizeof(DictState));

    pstate->curr_record = (Record*) MIDL_user_allocate(sizeof(Record));
    pstate->curr_record->name = (char*) MIDL_user_allocate(81 * sizeof(char));

    // initialize curr_record to "minus infinity" in the order
    pstate->curr_record->key = -1;
    strcpy(pstate->curr_record->name, "");
    pstate->ref_count = 0;

    return(pstate);
}

void free_state(DictState * state)
{
    if (state != NULL) {
        if (state->curr_record != NULL) {
            if (state->curr_record->name != NULL)
                MIDL_user_free(state->curr_record->name);
            MIDL_user_free(state->curr_record);
        }
        MIDL_user_free(state);
    }
}

/*************************************************************************/
/***                     Rdict Duplicate utilities                     ***/
/*************************************************************************/

RDict *
RDict_Duplicate(
    IN RDict * src
    )
{
    RDict * dst = (RDict*) MIDL_user_allocate(sizeof(RDict));

    dst->root = (RecordTreeNode*) Tree_Duplicate((TreeNode*)src->root);
    dst->size = src->size;
    dst->state = DictState_Duplicate(src->state);

    return(dst);
}

DictState *
DictState_Duplicate(
    IN DictState * src
    )
{
    DictState * dst = (DictState*) MIDL_user_allocate(sizeof(DictState));

    dst->curr_record = ItemDuplicate(src->curr_record);
    dst->ref_count = src->ref_count;

    return(dst);
}

TreeNode *
TreeNode_Duplicate(
    IN TreeNode * src
    )
{
    TreeNode * pnode = (TreeNode*) MIDL_user_allocate(sizeof(TreeNode));

    pnode->left = pnode->right = NULL;
    pnode->item = ItemDuplicate(src->item);

    return(pnode);
}

TreeNode *
Tree_Duplicate(
    IN TreeNode * src
    )
{
    TreeNode * dst;

    if (src == NULL)
        return((TreeNode*)NULL);

    dst = TreeNode_Duplicate(src);
    dst->left = Tree_Duplicate(src->left);
    dst->right = Tree_Duplicate(src->right);
    return(dst);
}

/*************************************************************************/
/***                MIDL_user_allocate / MIDL_user_free                ***/
/*************************************************************************/

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t count)
{
    return(malloc(count));
}

void __RPC_USER MIDL_user_free(void __RPC_FAR * p)
{
    free(p);
}

/*************************************************************************/
/***                          Utility functions                        ***/
/*************************************************************************/

/*  In the most general case *cmp is a two argument function:
    (*cmp)(void *item0, void *item1) which compares two items,
    and returns:    -1 if item0 < item1;
                     0 if item0 == item1;
                    +1 if item0 > item1.
    The common case is: each item has a field named "key";
    item.key is of type long, or string.
*/

int
comp(void* x, void* y)
{
    int res = ((Record*)x)->key - ((Record*)y)->key;

    if (res == 0)
        return( strcmp( ((Record*)x)->name, ((Record*)y)->name ) );
    else
        return( res ) ;
}

Record *
ItemDuplicate(
    Record * item
    )
{
    if (item == NULL)
        return(NULL);

    return( makeRecord( item->key, item->name ) );
}

Record *
makeRecord(
    short key,
    char * name
    )
{
    Record * pr = (Record*) MIDL_user_allocate(sizeof(Record));

    if (!name)
        printf("makeRecord: NULL name\n");

    pr->name = (char*) MIDL_user_allocate(strlen(name)+1);
    strcpy(pr->name, name);
    pr->key = key;

    return(pr);
}

void
freeRecord(
    Record * pr
    )
{
    if (pr != NULL) {
        if (pr->name != NULL)
            MIDL_user_free(pr->name);
        MIDL_user_free(pr);
    }
}

void
ItemCopy(
    IN Record * src,
    OUT Record * dest
    )
{
    int i;

    dest->key = src->key;

    // copy name, trubcated to 80 characters
    for(i=0 ; (src->name[i] != '\0') && (i<80) ; i++)
        dest->name[i]=src->name[i];

    dest->name[i]='\0';
}

void
printRecord(void* rp)
{
    printf("%d : %s\n", ((Record*)rp)->key, ((Record*)rp)->name);
}

void
Dict_Print(             /* prints the binary tree (indented right subtree,
                           followed by the root, followed by the indented
                           right dubtree) */
    Dictionary * dp,
    int indent)         /* number of spaces to indent subsequent levels */
{
    prinTree(0, indent, dp->root, dp->print_rec);
}

char spaces[] =
"                                                                                                                                                                                                                                                       ";

void
prinTree(
    int lmargin,             /* indentation of the root of the tree     */
    int indent,              /* indentation of subsequent levels        */
    TreeNode *np,            /* pointer to the root node                */
    PrintFun print)          /* short, one line, record print routine   */
{
    if (np == NULL)
        return;

    prinTree(lmargin+indent, indent, np->right, print);

    if (lmargin > sizeof(spaces))
        lmargin = sizeof(spaces);;

    spaces[lmargin] = 0;
    printf(spaces);
    spaces[lmargin] = ' ';

    (*print)(np->item);

    prinTree(lmargin+indent, indent, np->left, print);

}

TreeNode*
makeNode(void * item)
{
    TreeNode* tp;

    tp = (TreeNode*)MIDL_user_allocate(sizeof(TreeNode));
    tp->item = item;
    tp->left = tp->right = NULL;

    return(tp);
}
