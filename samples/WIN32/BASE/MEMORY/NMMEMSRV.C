
/******************************************************************************\
*       This is a part of the Microsoft Source Code Samples. 
*       Copyright (C) 1993 Microsoft Corporation.
*       All rights reserved. 
*       This source code is only intended as a supplement to 
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the 
*       Microsoft samples programs.
\******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>
#include <string.h>
#include <winbase.h>

extern ErrorOut();

HANDLE CreateMapFile (char *MapFileName);
HANDLE CreateMap     (HANDLE *FileToBeMapped, char MapName[30]);
LPVOID MapView       (HANDLE *hMap);

/**************************************************************************
 * HANDLE CreateMapFile(char *MapFileName)
 *
 * Purpose: Create a Map file to map named share memory
 *
 * Inputs:  none
 *
 * Returns: MapFileHandle - a handle to the file
 *                        or NULL if failure
 *
 * Calls:   CreateFile, ErrorOut
 *
\**************************************************************************/
HANDLE CreateMapFile(char *MapFileName)
{
//char MapFileName[30]="MapFile.001";
HANDLE MapFileHandle;

MapFileHandle= CreateFile(MapFileName,
                          GENERIC_READ | GENERIC_WRITE,
                          FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL,
                          CREATE_ALWAYS,
                          FILE_ATTRIBUTE_NORMAL /* | STANDARD_RIGHTS_REQUIRED |
                          FILE_MAP_WRITE | FILE_MAP_READ */,
                          NULL);

if (MapFileHandle == (HANDLE)-1)
  {
  ErrorOut("CreateFile");
  return(NULL);
  }
else
  return(MapFileHandle);

}

/**************************************************************************
 * HANDLE CreateMap(HANDLE *FileToBeMapped, char MapName[30] )
 *
 * Purpose: Create File Mapping object using the open file handle
 *
 * Inputs:  *FileToBeMapped - pointer to the file handle
 *
 * Returns: MapHandle - handle to the file mapping object
 *                    or NULL if failure
 *
 * Calls:   CreateFileMapping, ErrorOut
 *
 * History:
 * 09-13-91 Pete Gray   Created.
 *
\**************************************************************************/

HANDLE CreateMap(HANDLE *FileToBeMapped, char MapName[30])
{
//char MapNameH[30]="MapName1";
HANDLE MapHandle;

MapHandle= CreateFileMapping(*FileToBeMapped,
                             NULL,
                             PAGE_READWRITE,
                             0,
                             4096,
                             MapName);

if (MapHandle == NULL)
  {
  ErrorOut("CreateFileMapping");
  return(NULL);
  }
else
  return(MapHandle);

}


/**************************************************************************
 * LPVOID MapView(HANDLE *hMap)
 *
 * Purpose: Map the file mapping object into address space
 *
 * Inputs:  *hMap - pointer to the mapping object
 *
 * Returns: MappedPointer - pointer to the address space that the
 *                        object is mapped into
 *                        or NULL if failure
 *
 * Calls:   MapViewOfFile, ErrorOut
 *
 * History:
 * 09-13-91 Pete Gray   Created.
 *
\**************************************************************************/

LPVOID MapView(HANDLE *hMap)
{
LPVOID MappedPointer;

MappedPointer= MapViewOfFile(*hMap,
                             FILE_MAP_WRITE | FILE_MAP_READ,
                             0,
                             0,
                             4096);
if (MappedPointer == NULL)
  {
  ErrorOut("MapViewOfFile");
  return(NULL);
  }
else
  return(MappedPointer);

}
