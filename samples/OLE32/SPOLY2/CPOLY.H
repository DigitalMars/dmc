/*** 
*cpoly.h
*
*  This is a part of the Microsoft Source Code Samples.
*
*  Copyright (C) 1992-1995 Microsoft Corporation. All rights reserved.
*
*  This source code is only intended as a supplement to Microsoft Development
*  Tools and/or WinHelp documentation.  See these sources for detailed
*  information regarding the Microsoft samples programs.
*
*Purpose:
*  Definition of the CPoly class.
*
*  The CPoly class defines a number of methods and exposes them for
*  external programmability via IDispatch,
*
*  methods:
*    DRAW		- draw the polygon
*    RESET		- delete all points from the polygon
*
*    ADDPOINT(X, Y)	- add a point with coordinates (x,y) to the polygon
*
*    ENUMPOINTS		- return a collection of the polygon's points
*
*    GETXORIGIN		- get and set the X origin of the polygon
*    SETXORIGIN
*
*    GETYORIGIN		- get and set the Y origin of the polygon
*    SETYORIGIN
*
*    GETWIDTH		- get and set the line width of the polygon
*    SETWIDTH
*
*  UNDONE: update description
*
*Implementation Notes:
*
*****************************************************************************/


#ifndef	CLASS
# ifdef	__TURBOC__
#  define CLASS class huge
# else
#  define CLASS class FAR
# endif
#endif

class CPoint;

CLASS CPoly : public IDispatch
{
public:
    static CPoly FAR* Create();

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppvObj);
    STDMETHOD_(unsigned long, AddRef)(void);
    STDMETHOD_(unsigned long, Release)(void);

    /* IDispatch methods */
    STDMETHOD(GetTypeInfoCount)(unsigned int FAR* pcTypeInfo);

    STDMETHOD(GetTypeInfo)(
      unsigned int iTypeInfo,
      LCID lcid,
      ITypeInfo FAR* FAR* ppTypeInfo);

    STDMETHOD(GetIDsOfNames)(
      REFIID riid,
      OLECHAR FAR* FAR* rgszNames,
      unsigned int cNames,
      LCID lcid,
      DISPID FAR* rgdispid);

    STDMETHOD(Invoke)(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      unsigned short wFlags,
      DISPPARAMS FAR* pdispparams,
      VARIANT FAR* pvarResult,
      EXCEPINFO FAR* pexcepinfo,
      unsigned int FAR* puArgErr);

    /* Introduced methods */

    virtual void  METHODCALLTYPE EXPORT Draw(void);
    virtual void  METHODCALLTYPE EXPORT Reset(void);

    // add a point with the given 'x' and 'y' coordinates
    virtual HRESULT METHODCALLTYPE EXPORT AddPoint(short x, short y);

    // return a collection of the polygon's points
    virtual IUnknown FAR* METHODCALLTYPE EXPORT EnumPoints(void);

    // get/set the polygon's X origin property
    virtual short METHODCALLTYPE EXPORT GetXOrigin(void);
    virtual void  METHODCALLTYPE EXPORT SetXOrigin(short x);

    // get/set the polygon's Y origin property
    virtual short METHODCALLTYPE EXPORT GetYOrigin(void);
    virtual void  METHODCALLTYPE EXPORT SetYOrigin(short y);

    virtual short METHODCALLTYPE EXPORT GetWidth(void);
    virtual void  METHODCALLTYPE EXPORT SetWidth(short width);

    virtual short METHODCALLTYPE EXPORT get_red(void);
    virtual void  METHODCALLTYPE EXPORT set_red(short red);

    virtual short METHODCALLTYPE EXPORT get_green(void);
    virtual void  METHODCALLTYPE EXPORT set_green(short green);

    virtual short METHODCALLTYPE EXPORT get_blue(void);
    virtual void  METHODCALLTYPE EXPORT set_blue(short blue);

    // Debug method
    virtual void  METHODCALLTYPE EXPORT Dump(void);
    virtual void  METHODCALLTYPE EXPORT Quit(void);

public: 

    // Draw all polygons.
    static void PolyDraw(void);

    // Release all polygons.
    static void PolyTerm(void);

    // Dump all polygons to dbwin.
    static void PolyDump(void);


private:
    CPoly();

    short m_xorg;
    short m_yorg;
    short m_width;

    short m_red;
    short m_green;
    short m_blue;

    unsigned long m_refs;
    unsigned int m_cPoints;

    ITypeInfo FAR* m_ptinfo;

    POINTLINK FAR* m_ppointlink;
    POINTLINK FAR* m_ppointlinkLast;
};

// DISPIDs for the members and properties available via IDispatch.
//
enum IDMEMBER_CPOLY {
    IDMEMBER_CPOLY_DRAW = 1,
    IDMEMBER_CPOLY_RESET,
    IDMEMBER_CPOLY_ADDPOINT,
    IDMEMBER_CPOLY_ENUMPOINTS,
    IDMEMBER_CPOLY_GETXORIGIN,
    IDMEMBER_CPOLY_SETXORIGIN,
    IDMEMBER_CPOLY_GETYORIGIN,
    IDMEMBER_CPOLY_SETYORIGIN,
    IDMEMBER_CPOLY_GETWIDTH,
    IDMEMBER_CPOLY_SETWIDTH,
    IDMEMBER_CPOLY_GETRED,
    IDMEMBER_CPOLY_SETRED,
    IDMEMBER_CPOLY_GETGREEN,
    IDMEMBER_CPOLY_SETGREEN,
    IDMEMBER_CPOLY_GETBLUE,
    IDMEMBER_CPOLY_SETBLUE,
    IDMEMBER_CPOLY_DUMP,
    IDMEMBER_CPOLY_QUIT,
    IDMEMBER_CPOLY_MAX
};

// CPoly method indices
//
enum IMETH_CPOLY {
    IMETH_CPOLY_QUERYINTERFACE = 0,
    IMETH_CPOLY_ADDREF,
    IMETH_CPOLY_RELEASE,
    IMETH_CPOLY_GETTYPEINFOCOUNT,
    IMETH_CPOLY_GETTYPEINFO,
    IMETH_CPOLY_GETIDSOFNAMES,
    IMETH_CPOLY_INVOKE,
    IMETH_CPOLY_DRAW,
    IMETH_CPOLY_RESET,
    IMETH_CPOLY_ADDPOINT,
    IMETH_CPOLY_ENUMPOINTS,
    IMETH_CPOLY_GETXORIGIN,
    IMETH_CPOLY_SETXORIGIN,
    IMETH_CPOLY_GETYORIGIN,
    IMETH_CPOLY_SETYORIGIN,
    IMETH_CPOLY_GETWIDTH,
    IMETH_CPOLY_SETWIDTH,
    IMETH_CPOLY_GETRED,
    IMETH_CPOLY_SETRED,
    IMETH_CPOLY_GETGREEN,
    IMETH_CPOLY_SETGREEN,
    IMETH_CPOLY_GETBLUE,
    IMETH_CPOLY_SETBLUE,
    IMETH_CPOLY_DUMP,
    IMETH_CPOLY_QUIT,
    IMETH_CPOLY_MAX
};

// structure used to link together polygons
//
struct POLYLINK {
    POLYLINK FAR* next;
    CPoly FAR* ppoly;
};


// The CPoly class factory
//
CLASS CPolyCF : public IClassFactory
{
public:
    static IClassFactory FAR* Create();

    /* IUnknown methods */
    STDMETHOD(QueryInterface)(REFIID riid, void FAR* FAR* ppv);
    STDMETHOD_(unsigned long, AddRef)(void);
    STDMETHOD_(unsigned long, Release)(void);

    /* IClassFactory methods */
    STDMETHOD(CreateInstance)(
      IUnknown FAR* pUnkOuter, REFIID riid, void FAR* FAR* ppv);
#ifdef _MAC
    STDMETHOD(LockServer)(unsigned long fLock);
#else
    STDMETHOD(LockServer)(BOOL fLock);
#endif

private:
    CPolyCF();

    unsigned long m_refs;
};

