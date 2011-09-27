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
 *  curve.c
 *  This program uses the Utility Library NURBS routines to
 *  draw a one-dimensional NURBS curve.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

GLUnurbsObj *theNurb;

void myinit(void)
{
    glShadeModel (GL_FLAT);
    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty (theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
}

/*  This routine draws a B-spline curve.  Try a different 
 *  knot sequence for a Bezier curve.  For example,
    GLfloat knots[8] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
 */

void CALLBACK display(void)
{
    GLfloat ctlpoints[4][3] = {{-.75, -.75, 0.0}, 
    {-.5, .75, 0.0}, {.5, .75, 0.0}, {.75, -.75, 0.0}};
    GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0}; 

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    gluBeginCurve(theNurb);
    gluNurbsCurve(theNurb, 
        8, knots,
        3,
        &ctlpoints[0][0], 
        4,
        GL_MAP1_VERTEX_3);
    gluEndCurve(theNurb);
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
    gluOrtho2D (-1.0, 1.0, -1.0 * (GLfloat) h/(GLfloat) w, 
        1.0 * (GLfloat) h/(GLfloat) w);
    else
    gluOrtho2D (-1.0 * (GLfloat) w/(GLfloat) h,
        1.0 * (GLfloat) w/(GLfloat) h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*  Main Loop 
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("NURBS Curve");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
