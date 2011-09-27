/* TMLDOS.h
*/

/* Escape code for information embedded in text
*/
#define ESCAPECODE 0x1b
#define EOLCODE    0x0a

/**************************** typedefs
*/
#define FAR __far
typedef unsigned char      BYTE;
typedef unsigned short int WORD;
typedef unsigned long  int DWORD;

/**************************** externs
*/
extern char *pszTitle;         /* pointer to document title */
extern char szFilename[];      /* file name, path stripped  */
extern char FAR *fpOutBufPtr;  /* pointer to output buffer  */

void ChangeToFileDir(char *);
int Parse (FILE *, int, char **, char FAR **, char **, int *, int);
void Display();