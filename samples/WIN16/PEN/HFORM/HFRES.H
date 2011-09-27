/************************************************************

   Resource header file for HFORM.C


************************************************************/

#define menuMain     1
#define miFile       100
#define miExit       101
#define miClearAll   102
#define miSampleDlg  103
#define miLoad       104
#define miSave       105

/*
   miNextField and miPrecField are used as menu accelerators
   to move from one field to another using the TAB and SHIFT-TAB
   keystrokes.
*/

#define miNextField  115
#define miPrecField  116
#define miLastInk    117

#define szDelay      "Delay Directions &Recognition"
#define szRecog      "&Recognize Directions Now"

#define iconHform    100

#define IDC_SAMPLE   400
#define IDC_EDIT     401

#define rsNoPW       1000  // stringtable

#ifdef RC_INVOKED
#define ID(id) id
#else
#define ID(id) MAKEINTRESOURCE(id)
#endif

/* resource ID's */
#define IDHFORM  ID(1)
