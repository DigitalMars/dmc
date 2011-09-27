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
 *  antipoint.c
 *  The program draws antialiased points, in RGBA mode.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

/*  Initialize point anti-aliasing for RGBA mode, including alpha
 *  blending, hint, and point size.  These points are 3.0 pixels big.
 */
void myinit(void)
{
    glEnable (GL_POINT_SMOOTH);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
    glPointSize (3.0);

    glClearColor(0.0, 0.0, 0.0, 0.0);
}

/*  display() draws several points.
 */
void CALLBACK display(void)
{
    int i;

    glClear (GL_COLOR_BUFFER_BIT);
    glColor4f (1.0, 1.0, 1.0, 1.0);
    glBegin (GL_POINTS);
    for (i = 1; i < 10; i++) {
        glVertex2f ((GLfloat) i * 10.0, (GLfloat) i * 10.0);
    }
    glEnd ();
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w < h) 
    glOrtho (0.0, 100.0, 
        0.0, 100.0*(GLfloat) h/(GLfloat) w, -1.0, 1.0);
    else
    glOrtho (0.0, 100.0*(GLfloat) w/(GLfloat) h, 
        0.0, 100.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 100, 100);
    auxInitWindow ("Points");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
