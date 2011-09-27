/*
Copyright Prototronics, 1987
Totem Lake P.O. 8117
Kirkland, Washington 98034

(206) 820-1972

Licensed to Digital Mars. */
/*
June 11, 1987 from Ray Gardner's,
(Denver, Colorado) public domain version. */ 

/*    qsort()  --  Quicksort function
**
**    Usage:   qsort(base, nbr_elements, width_bytes, compare_function);
**                char *base;
**                unsigned int nbr_elements, width_bytes;
**                int (*compare_function)();
**
**    Sorts an array starting at base, of length nbr_elements, each
**    element of size width_bytes, ordered via compare_function; which
**    is called as  (*compare_function)(ptr_to_element1, ptr_to_element2)
**    and returns < 0 if element1 < element2, 0 if element1 = element2,
**    > 0 if element1 > element2.  Most of the refinements are due to
**    R. Sedgewick.  See "Implementing Quicksort Programs", Comm. ACM,
**    Oct. 1978, and Corrigendum, Comm. ACM, June 1979.
*/

static void near pascal iswap (unsigned, int *, int *);
static void near pascal cswap (unsigned, char *, char *);

int _maxspan = 7;               /* subfiles of _maxspan or fewer elements */
                                /* will be sorted by a simple insertion sort */

/* Adjust _maxspan according to relative cost of a swap and a compare.  Reduce 
_maxspan (not less than 1) if a swap is very expensive such as when you have 
an array of large structures to be sorted, rather than an array of pointers to 
structures.  The default value is optimized for a high cost for compares. */ 

#define SWAP(a,b) (*swap_fp)(n_to_swap,a,b)
#define COMP(a,b) (*comp_fp)(a,b)

void qsort (base, nel, width, comp_fp)
void *base;                            /* base address of array to sort  */
unsigned int nel, width;               /* nbr elements, size of element  */
int (*comp_fp)(const void *, const void *);        /* compare function               */
{
  char *stack[40], **sp;                 /* stack and stack pointer        */
  char *i, *j, *limit;                   /* scan and limit pointers        */
  unsigned n_to_swap;			 /* nbr of chars or ints to swap   */
  unsigned thresh;                       /* size of _maxspan elements in   */
					 /* bytes */
  typedef void (near pascal *swap_t) (unsigned, void *, void *);
  swap_t swap_fp;

  if (width % sizeof(int) != 0)
  {
    swap_fp = (swap_t)cswap;
    n_to_swap = width;
  }
  else
  {
    swap_fp = (swap_t)iswap;
    n_to_swap = width / sizeof(int);
  }

  thresh = _maxspan * width;             /* init threshold                 */
  sp = stack;                            /* init stack pointer             */
  limit = (char *)base + nel * width;            /* pointer past end of array      */
  while (1)                              /* repeat until done then return  */
  {
    while (limit - (char *)base > thresh)        /* if more than _maxspan elements */
    {
      /*swap middle, base*/
      SWAP (((unsigned)(limit - (char *)base) >> 1) -
           ((((unsigned)(limit - (char *)base) >> 1)) % width) + (char *)base, (char *)base);

      i = (char *)base + width;                /* i scans from left to right     */
      j = limit - width;               /* j scans from right to left     */

      if ( COMP(i, j) > 0 )            /* Sedgewick's                    */
        SWAP(i, j);                    /*    three-element sort          */
      if ( COMP((char *)base, j) > 0 )         /*        sets things up          */
        SWAP((char *)base, j);                 /*            so that             */
      if ( COMP(i, (char *)base) > 0 )         /*              *i <= *base <= *j */
        SWAP(i, (char *)base);                 /* *base is the pivot element     */

      while (1)
      {
        do                            /* move i right until *i >= pivot */
          i += width;
        while (COMP (i, (char *)base) < 0);
        do                            /* move j left until *j <= pivot  */
          j -= width;
        while (COMP (j, (char *)base) > 0);
        if (i > j)                    /* break loop if pointers crossed */
          break;
        SWAP (i, j);                  /* else swap elements, keep scanning */
      }
      SWAP ((char *)base, j);                  /* move pivot into correct place  */
      if (j - (char *)base > limit - i)        /* if left subfile is larger...   */
      {
        sp[0] = (char *)base;                 /* stack left subfile base        */
        sp[1] = j;                    /*    and limit                   */
        (char *)base = i;                     /* sort the right subfile         */
      }
      else                             /* else right subfile is larger   */
      {
        sp[0] = i;                    /* stack right subfile base       */
        sp[1] = limit;                /*    and limit                   */
        limit = j;                    /* sort the left subfile          */
      }
      sp += 2;                        /* increment stack pointer        */
    }

    /* Insertion sort on remaining subfile. */
    i = (char *)base + width;
    while (i < limit)
    {
      j = i;
      while (j > (char *)base && COMP (j - width, j) > 0)
      {
        SWAP (j - width, j);
        j -= width;
      }
      i += width;
    }

    if (sp > stack)    /* if any entries on stack...     */
    {
      sp -= 2;         /* pop the base and limit         */
      base = sp[0];
      limit = sp[1];
    }
    else              /* else stack empty, all done     */
      break;          /* Return. */
  }
}

static void near pascal iswap (unsigned k, int *a, int *b)  /* swap ints */
{
  int tmp;

  do 
  {
    tmp = *a;
    *a = *b;
    *b = tmp;
    a++; b++;
  } while (--k);
}

static void near pascal cswap (unsigned k, char *a, char *b)    /* swap chars */
{
  char tmp;

  do
  {
    tmp = *a;
    *a = *b;
    *b = tmp;
    a++; b++;
  } while (--k);
}
