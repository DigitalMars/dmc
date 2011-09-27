/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *               1993-1995 Microsoft Corporation
 *
 * ALL RIGHTS RESERVED
 *
 * Please refer to OpenGL/readme.txt for additional information
 *
 */

#ifdef GLX_MOTIF
void intToggleCB(Widget w, XtPointer client_data, XtPointer call_data);
void resetLightsCB(Widget w);
void autoMotionCB(Widget w, XtPointer client_data, XtPointer call_data);
void initCB(Widget w);
void exposeCB(Widget w);
void resizeCB(Widget w, XtPointer client_data, XtPointer call);
void inputCB(Widget w, XtPointer client_data, XtPointer call_data);
void drawAllCB(Widget w);
void drawSomethingCB(Widget w, XtPointer client_data, XtPointer call_data);
void refractionCB(Widget w, XtPointer client_data, XtPointer call_data);
void subdivisionCB(Widget w, XtPointer client_data, XtPointer call_data);
void light_onCB(Widget w, XtPointer client_data, XtPointer call_data);
void exitCB(Widget w, XtPointer client_data, XtPointer call_data);
Boolean drawWP(XtPointer data);
#endif

void CALLBACK draw(void);
void CALLBACK vQuickMove(void);
void CALLBACK vResetLights(void);
void CALLBACK vAutoMotion(void);
void vInit(void);
void vExpose(int, int);
void CALLBACK vResize(GLsizei, GLsizei);
void CALLBACK vDrawAll(void);
void CALLBACK vDrawSquare(void);
void CALLBACK vDrawShadow(void);
void CALLBACK vDrawRefraction(void);
void CALLBACK vDrawSphere(void);
void CALLBACK vDrawLight(void);
void CALLBACK vDrawTexture(void);
void vDrawStuff(int *);
void CALLBACK vRefractionAIR(void);
void CALLBACK vRefractionICE(void);
void CALLBACK vRefractionWATER(void);
void CALLBACK vRefractionZincGLASS(void);
void CALLBACK vRefractionLightGLASS(void);
void CALLBACK vRefractionHeavyGLASS(void);
void vRefraction(int);
void CALLBACK vSubdivision10(void);
void CALLBACK vSubdivision20(void);
void CALLBACK vSubdivision30(void);
void CALLBACK vSubdivision40(void);
void vSubdivision(int);
void CALLBACK vRLight_on(void);
void CALLBACK vGLight_on(void);
void CALLBACK vBLight_on(void);
void vLight_on(int);
void vExit(void);
GLboolean bAutoMotion(void);

void CALLBACK vMouseDown(AUX_EVENTREC *);
void CALLBACK vLeftMouseUp(AUX_EVENTREC *);
void CALLBACK vMiddleMouseUp(AUX_EVENTREC *);
void CALLBACK vRightMouseUp(AUX_EVENTREC *);
void CALLBACK vMouseMove(AUX_EVENTREC *event);



PVOID __nw(unsigned int ui);
VOID __dl(PVOID pv);
PVOID __vec_new(void *p, int x, int y, void *q);
