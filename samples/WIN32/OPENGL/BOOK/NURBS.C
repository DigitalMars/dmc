/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *               1993-1995 Microsoft Corporation
 *
 * ALL RIGHTS RESERVED
 *
 * Please refer to OpenGL/readme.txt for additional information
 *
 */

/*
 *  nurbs.c
 *  This program shows a NURBS (Non-uniform rational B-splines)
 *  surface, shaped like a heart.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

#define S_NUMPOINTS 13
#define S_ORDER     3   
#define S_NUMKNOTS  (S_NUMPOINTS + S_ORDER)
#define T_NUMPOINTS 3
#define T_ORDER     3 
#define T_NUMKNOTS  (T_NUMPOINTS + T_ORDER)
#define SQRT2    1.41421356237309504880

/* initialized local data */

GLfloat sknots[S_NUMKNOTS] =
    {-1.0, -1.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0,
      4.0,  5.0,  6.0, 7.0, 8.0, 9.0, 9.0, 9.0};
GLfloat tknots[T_NUMKNOTS] = {1.0, 1.0, 1.0, 2.0, 2.0, 2.0};

GLfloat ctlpoints[S_NUMPOINTS][T_NUMPOINTS][4] = {
{   {4.,2.,2.,1.},{4.,1.6,2.5,1.},{4.,2.,3.0,1.}    },
{   {5.,4.,2.,1.},{5.,4.,2.5,1.},{5.,4.,3.0,1.} },
{   {6.,5.,2.,1.},{6.,5.,2.5,1.},{6.,5.,3.0,1.} },
{   {SQRT2*6.,SQRT2*6.,SQRT2*2.,SQRT2},
    {SQRT2*6.,SQRT2*6.,SQRT2*2.5,SQRT2},
    {SQRT2*6.,SQRT2*6.,SQRT2*3.0,SQRT2}  },
{   {5.2,6.7,2.,1.},{5.2,6.7,2.5,1.},{5.2,6.7,3.0,1.}   },
{   {SQRT2*4.,SQRT2*6.,SQRT2*2.,SQRT2},
    {SQRT2*4.,SQRT2*6.,SQRT2*2.5,SQRT2},
    {SQRT2*4.,SQRT2*6.,SQRT2*3.0,SQRT2}  },
{   {4.,5.2,2.,1.},{4.,4.6,2.5,1.},{4.,5.2,3.0,1.}  },
{   {SQRT2*4.,SQRT2*6.,SQRT2*2.,SQRT2},
    {SQRT2*4.,SQRT2*6.,SQRT2*2.5,SQRT2},
    {SQRT2*4.,SQRT2*6.,SQRT2*3.0,SQRT2}  },
{   {2.8,6.7,2.,1.},{2.8,6.7,2.5,1.},{2.8,6.7,3.0,1.}   },
{   {SQRT2*2.,SQRT2*6.,SQRT2*2.,SQRT2},
    {SQRT2*2.,SQRT2*6.,SQRT2*2.5,SQRT2},
    {SQRT2*2.,SQRT2*6.,SQRT2*3.0,SQRT2}  },
{   {2.,5.,2.,1.},{2.,5.,2.5,1.},{2.,5.,3.0,1.} },
{   {3.,4.,2.,1.},{3.,4.,2.5,1.},{3.,4.,3.0,1.} },
{   {4.,2.,2.,1.},{4.,1.6,2.5,1.},{4.,2.,3.0,1.}    }
};

GLUnurbsObj *theNurb;

/*  Initialize material property, light source, lighting model, 
 *  and depth buffer.
 */
void myinit(void)
{
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 0.2, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    GLfloat light0_position[] = { 1.0, 0.1, 1.0, 0.0 };
    GLfloat light1_position[] = { -1.0, 0.1, 1.0, 0.0 };

    GLfloat lmodel_ambient[] = { 0.3, 0.3, 0.3, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);

    theNurb = gluNewNurbsRenderer();

    glNewList (1, GL_COMPILE);
    gluBeginSurface(theNurb);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
    gluNurbsSurface(theNurb, 
        S_NUMKNOTS, sknots,
        T_NUMKNOTS, tknots,
        4 * T_NUMPOINTS,
        4,
        &ctlpoints[0][0][0], 
        S_ORDER, T_ORDER,
        GL_MAP2_VERTEX_4);
    gluEndSurface(theNurb);
    glEndList ();

}

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslatef (4., 4.5, 2.5);
    glRotatef (220.0, 1., 0., 0.);
    glRotatef (115.0, 0., 1., 0.);
    glTranslatef (-4., -4.5, -2.5);
    glCallList (1);
    glPopMatrix();
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2.0, 2.0, -2.0, 2.0, 0.8, 10.0);
    gluLookAt(7.0,4.5,4.0, 4.5,4.5,2.0, 6.0,-3.0,2.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("NURBS Surface");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
