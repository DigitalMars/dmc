/*  clsid.c
 *
 *  This file allocates and initializes the CLSIDs.
 *
 */



// initguid.h requires this.
//
#include <compobj.h>

// this redefines the DEFINE_GUID() macro to do allocation.
//
#include <initguid.h>

// due to the previous header, including this causes the DEFINE_GUID
// definitions in the following header(s) to actually allocate data.
//
#include "clsid.h"
