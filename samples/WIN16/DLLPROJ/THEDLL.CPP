// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
//
//
//		File : thedll.cpp
//
//
//		Demonstrates simple dll containing file I/O, Windows
//	calls, and malloc.
//
//		This is C++ code only in the most Academic sense, and is
//		intended to demonstrate the exporting of member functions.
//
//
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- 

#ifndef __LARGE__
   #error  "This DLL must be compiled in Large Memory Model"
#endif

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thedll.hpp"

int test_class::x;
int test_class::y;

extern "C" int FAR PASCAL LibMain(HANDLE hInstance, WORD wDataSeg, WORD wHeapSize, LPSTR lpsxCmdLine);

int FAR PASCAL LibMain(HANDLE hInstance, WORD wDataSeg, WORD wHeapSize, LPSTR lpsxCmdLine)
{
  if (wHeapSize > 0)
    UnlockData(0);

  return 1;
}






void FAR _export test_class::DllFunc(HDC hdc, int mx, int my)
{
  HPEN hPen;
  static char str[80];	// must be DS relative.
  FILE *file;
  char *laststr;

// -=-=-=-=-=-=-=-=-=-=-  DRAW CIRCLE -=-=-=-=-=-=-=-=-=-=-


  hPen = SelectObject(hdc, GetStockObject(BLACK_PEN));
  x = mx;
  y = my;

  if ((x > 0) && (y > 0))
    Ellipse(hdc,x-10,y-10,x+10,y+10);

  SelectObject(hdc, hPen);

// -=-=-=-=-=-=-=-=-=-=-=- MALLOC & FILE I/O =-=-=-=-=-=-=-=-=-=-=-=-=

// Need large model for File I/O
  file = fopen("junk.txt","a");

  sprintf(str,"(%d,%d) ", x, y);

  laststr = (char *) malloc (20);

  strcpy(laststr,"Are the coords\n");
  strcat(str,laststr);

 fprintf(file,str);		// Will not work in S or M model - GP fault.
  free (laststr);
  fclose(file); 

}
