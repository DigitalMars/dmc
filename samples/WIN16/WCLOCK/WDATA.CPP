/*  Symantec C++ Demonstration Program
 *  Copyright (C) 1993 Symantec, Corp.
 */

#include <windows.h>
#include <string.h>
#include <malloc.h>


typedef struct datarec {
    struct  datarec far *left, far *right;
    int	    value;
    int     uc;
    char    dataString[128];
} datarec;

typedef struct intlist {
    int     val;
    struct  intlist *next;
} intlist;

typedef struct baselist {
    int     data;
    double  realval;
    intlist *squaresList;
    struct  baselist *next;
} baselist;


static struct array {
    int val;
    double d;
} array[128];


static int	count	= 0;
static int	lastElt = 0;
static baselist *base	= NULL;

datarec far *root;

//----------------- function prototypes ------------------------------

extern "C" {
  void InsertList(int i, double d);
  void InsertTree(datarec *rec);
  void AddEntry(int i, char *s);
  void BuildDataBase(void);
  void NextItem(datarec far *item);
  void TraverseTree(void);
};

//--------------------------------------------------------------------

void InsertList(int i, double d)
{
    intlist  *newi;
    baselist *newb;
    int j;
  
    newb = (baselist *)malloc(sizeof(baselist));
    newb->realval = d;
    newb->data    = i;

    for (j = 2; j < i; j *= j) {
        newi = (intlist *)malloc(sizeof(intlist));
        newi->val = j;
        newi->next = newb->squaresList;
        newb ->squaresList = newi;
    }
  
    newb->next = base;
    base      = newb;

    array[lastElt].val = i*2;
    array[lastElt].d   = d;
  
    ++lastElt;
}


void InsertTree(datarec *rec)
{
    datarec far *p;
  
    rec->left  = NULL;
    rec->right = NULL;
    rec->uc    = 0;

    if (root == NULL) {
        root = rec;
        return;
    }

    p = root;
    while (p != NULL) {
        if (rec->value < p->value) {
            if (p->left)
                p = p->left;
            else {
                p->left = rec;
                return;
            }
        } else {
            if (rec->value > p->value) {
                if (p->right)
                    p = p->right;
                else { 
                    p->right = rec;
                    return;
                }
            } else {
                ++rec->uc;
                return;
            }
        }
    }
}


void AddEntry(int i, char *s)
{
    datarec *newd;

    newd = (datarec *)malloc(sizeof(datarec));
    newd->value = i;
    strcpy(newd->dataString, s);
    InsertTree(newd);
  
    InsertList(i, (double)count/2);
    ++count;
}


void BuildDataBase(void)
{
    AddEntry( 50, "Symantec");
    AddEntry(100, "GDBX"      );
    AddEntry(  7, "DBX"       );
    AddEntry( 83, "BLIT"      );
    AddEntry( 55, "CodeView"  );
    AddEntry( 43, "DEBUG"     );
    AddEntry( 75, "DDT"       );
    AddEntry(  4, "SYMDEB"    );
    AddEntry( 12, "GDB"       );
    AddEntry( 99, "PMRTD"     );
    AddEntry( 99, "WRTDP"     );
    AddEntry( 88, "RTDWP"     );
}



void NextItem(datarec far *item)
{
    NextItem(item->left );
    NextItem(item->right);
}


void TraverseTree(void)
{
    NextItem(root);
}

 
