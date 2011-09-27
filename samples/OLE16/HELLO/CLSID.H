/*  clsid.h
 *
 *  This file defines the CLSIDs for the class which we
 *  will register with OLE.
 *
 *  You can get a universally unique identifier (uuid) by running
 *  the uuidgen tool supplied with OLE2.  When you run it, a huge
 *  hex number comes out.  Do some pattern matching and stick the
 *  numbers into this macro.  A long (l) is 8 he digits, a word (w)
 *  is 4 hex digits and a byte is two hex digits.
 *
 *  You only have to do this once for each class you create and register
 *  with OLE.
 *
 */



/*
 *
  IDispatch*    Hello.Application   {D3CE6D43-F1AF-1068-9FBB-08002B32372A}
  Type library: Hello.tlb       {D3CE6D44-F1AF-1068-9FBB-08002B32372A}
  Interface:    _IHello         {D3CE6D45-F1AF-1068-9FBB-08002B32372A}
  DispInterface:_DHello         {D3CE6D46-F1AF-1068-9FBB-08002B32372A}
 *
 */

DEFINE_GUID(CLSID_CHello,
				0xD3CE6D43,
				0xF1AF,
				0x1068,
				0x9F,
				0xBB,
				0x08,
				0x00,
				0x2B,
				0x32,
				0x37,
				0x2A);


DEFINE_GUID(IID_IHello,
				0xD3CE6D45,
				0xF1AF,
				0x1068,
				0x9F,
				0xBB,
				0x08,
				0x00,
				0x2B,
				0x32,
				0x37,
				0x2A);



DEFINE_GUID(IID_DHello,
				0xD3CE6D46,
				0xF1AF,
				0x1068,
				0x9F,
				0xBB,
				0x08,
				0x00,
				0x2B,
				0x32,
				0x37,
				0x2A);


DEFINE_GUID(LIBID_HELLOTLB,
				0xD3CE6D44,
				0xF1AF,
				0x1068,
				0x9F,
				0xBB,
				0x08,
				0x00,
				0x2B,
				0x32,
				0x37,
				0x2A);
