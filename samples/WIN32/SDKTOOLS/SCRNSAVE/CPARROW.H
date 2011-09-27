typedef struct
  {
    short lineup;             /* lineup/down, pageup/down are relative */
    short linedown;           /* changes.  top/bottom and the thumb    */
    short pageup;             /* elements are absolute locations, with */
    short pagedown;           /* top & bottom used as limits.          */
    short top;
    short bottom;
    short thumbpos;
    short thumbtrack;
    BYTE  flags;              /* flags set on return                   */
  } ARROWVSCROLL;
typedef ARROWVSCROLL NEAR     *NPARROWVSCROLL;
typedef ARROWVSCROLL FAR      *LPARROWVSCROLL;

#define UNKNOWNCOMMAND 1
#define OVERFLOW       2
#define UNDERFLOW      4

BOOL RegisterArrowClass (HANDLE hModule);
