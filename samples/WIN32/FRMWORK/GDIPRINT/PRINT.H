// print.h

// Stuff for "File.Print" dialog

typedef struct tagOPTIONS {
    int iOption;       // Print Options
    int iXScale;       // X scale factor (if iOption is DLGPRINT_SCALE)
    int iYScale;       // Y scale factor ('')
    } OPTIONSTRUCT, *LPOPTIONSTRUCT;
    
#define IDD_BESTFIT                 200
#define IDD_STRETCHTOPAGE           201
#define IDD_SCALE                   202 
#define IDD_XTEXT                   203
#define IDD_XAXIS                   204
#define IDD_YTEXT                   205
#define IDD_YAXIS                   206
