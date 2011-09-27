/*************************************************************/
/**                                                         **/
/**                 Microsoft RPC Examples                  **/
/**                 Dictionary Application                  **/
/**          Copyright(c) Microsoft Corp. 1992-1995         **/
/**                                                         **/
/*************************************************************/

/*
 *************************************************************************
 *                                                                       *
 * Local dictionary :play" example                                       *
 *                                                                       *
 * Description:                                                          *
 * This file contains a simple interactive loop of calls to the          *
 * dictionary.  The interface is identical to the remote dictionary      *
 * program described in the readme file.                                 *
 *                                                                       *
 *************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "dict0.h"
#include "play.h"
#include "util0.h"

#define TAB_STOPS 3


void Usage()
{
  printf("Usage : play \n\n");
  exit(1);
}

/*************************************************************************/
/***                     Remote Dictionary Test Loop                   ***/
/*************************************************************************/

void
Usage_Msg()
{
    printf("\nUsage: \nType a single character, followed by an optional key as follows:\n\n");
    printf("    i <key> :: Insert <key> into dictionary\n");
    printf("    d <key> :: Delete <key> from dictionary\n");
    printf("    f <key> :: Find <key> in dictionary\n");
    printf("    n       :: Next of local current item in dictionary\n");
    printf("    p       :: Previous of local current item in dictionary\n");
    printf("    h       :: Head (first item) of dictionary\n");
    printf("    t       :: Tail (last item) of dictionary\n");
    printf("    ?       :: Print this message\n");
    printf("    q       :: Quit\n\n");
    printf("where <key> is <integer> <string>\n");
}

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

void
TestLoop( Dictionary * pdict )
{
    char currName[80];
    char name[80];
    char op = 0;
    char buffer[80];

    Record r, currRecord;
    Record *pcurrRecord = &currRecord;
    Record *pr = &r;

    Dict_Status status;
    short i;

    pcurrRecord->name = currName;
    pr->name = name;

    Usage_Msg();

    while ( op != 'q' ) {

        printf("\nnext op (i d x f n p h t ? q): ");
        gets(buffer);
        op = buffer[0];

        if (op == 'i' || op == 'd' || op == 'f' ||
            op == '+' || op == '-' || op == 'I')
              sscanf(buffer+1, "%d %s", &pr->key, pr->name);

        switch (op) {
            case 'h':
                // get Head of list (first record);

                status = Dict_Next(pdict, NULL);
                ItemCopy(DICT_CURR_ITEM(pdict), pcurrRecord);
                ItemCopy(DICT_CURR_ITEM(pdict), pr);
                break;

            case 't':
                // get Tail of list (last record)

                status = Dict_Prev(pdict, NULL);
                ItemCopy(DICT_CURR_ITEM(pdict), pcurrRecord);
                ItemCopy(DICT_CURR_ITEM(pdict), pr);
                break;

            case 'n':
                // get Next record

                status = Dict_Next(pdict, pcurrRecord);
                ItemCopy(DICT_CURR_ITEM(pdict), pcurrRecord);
                break;

            case 'p':
                // get Previous record

                status = Dict_Prev(pdict, pcurrRecord);
                ItemCopy(DICT_CURR_ITEM(pdict), pcurrRecord);
                break;

            case 'N':
                // get Next record

                status = Dict_Next(pdict, pr);
                ItemCopy(DICT_CURR_ITEM(pdict), pr);
                break;

            case 'P':
                // get Previous record

                status = Dict_Prev(pdict, pr);
                ItemCopy(DICT_CURR_ITEM(pdict), pr);
                break;

            case 'r':
                ItemCopy(DICT_CURR_ITEM(pdict), pcurrRecord);
                break;


            case '+':
                // get Next record

                status = Dict_Next(pdict, pr);
                break;

            case '-':
                // get Previous record

                status = Dict_Prev(pdict, pr);
                break;

            case 'f':
                // Find <key>

                status = Dict_Find(pdict, pr);
                break;

            case 'i':
                // Insert <key>

                status = Dict_Insert(pdict,
                                     makeRecord(pr->key, pr->name) );
                break;

            case 'I':
                // Insert (<num'>,<name>) for all num': 3 < num' < num

                for (i = 3; i < pr->key; i++) {
                    status = Dict_Insert(pdict,
                                         makeRecord(i, pr->name) );
                }
                break;

            case 'd':
                // Delete <key>

                if (pdict != NULL) {
                    status = Dict_Delete(pdict, (void **)&pr);
                    freeRecord(pr);
                    pr = &r;
                }
                break;

            case 'x':
                // Delete DICT_CURR_ITEM

                if ((pdict != NULL) && (pdict->root != NULL)) {
                    pr = DICT_CURR_ITEM(pdict);
                    status = Dict_Delete(pdict, (void **) &pr);
                    freeRecord(pr);
                    pr = &r;
                }
                break;

            case 'X':
                // Empty the whole dictionary

                RecordTreeNodeFree((RecordTreeNode*)pdict->root);
                pdict->root = NULL;
                pr = &r;
                break;

            case '?':
                Usage_Msg();
                break;
        }

        if (op != '?' && op != 'q')
            Dict_Print(pdict, TAB_STOPS);
    }
}

Dict_Status
Dict_New_Dict(OUT Dictionary ** ppdict)
{
    static Dictionary * pdict;

    pdict = Dict_New(comp, tdSplay, printRecord);
    Init_dict(pdict);

    *ppdict = pdict;
    return(DICT_SUCCESS);
}

void
Init_dict(Dictionary * dp)
{
    Record* rp;

    rp = makeRecord((short)0, "jack_smith"); Dict_Insert(dp, rp);
    rp = makeRecord((short)0, "john_doe"); Dict_Insert(dp, rp);
    rp = makeRecord((short)1, "jean_doe"); Dict_Insert(dp, rp);
    rp = makeRecord((short)0, "joana_smith"); Dict_Insert(dp, rp);
    rp = makeRecord((short)1, "michael_jones"); Dict_Insert(dp, rp);
    rp = makeRecord((short)0, "mike_jacobs"); Dict_Insert(dp, rp);
    rp = makeRecord((short)2, "bill_jackson"); Dict_Insert(dp, rp);
    rp = makeRecord((short)0, "jane_doe"); Dict_Insert(dp, rp);
    rp = makeRecord((short)0, "dianne_jackson"); Dict_Insert(dp, rp);
    rp = makeRecord((short)1, "james_doe"); Dict_Insert(dp, rp);
    rp = makeRecord((short)1, "steve_johnson"); Dict_Insert(dp, rp);
    rp = makeRecord((short)2, "debbie_jones"); Dict_Insert(dp, rp);
    rp = makeRecord((short)0, "jacob_jacobson"); Dict_Insert(dp, rp);

    Dict_Print(dp, TAB_STOPS);
}

/*************************************************************************/
/***                             Main Loop                             ***/
/*************************************************************************/

void
main_dict ()
{
    Dictionary * pdict;
    Dictionary ** ppdict = &pdict;

    printf("getting a new dict\n");
    Dict_New_Dict(ppdict);
    printf("gotten a new dict in main_dict\n");
    TestLoop(pdict);
}

void _CRTAPI1
main(int argc, char *argv[])
{
    main_dict();
}
