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
 *  antipindex.c
 *  The program draws antialiased points, 
 *  in color index mode.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

#define RAMPSIZE 16
#define RAMPSTART 32

/*  Initialize point antialiasing for color index mode, 
 *  including loading a grey color ramp starting at 
 *  RAMPSTART, which must be a multiple of 16.
 */
void myinit(void)
{
    int i;

    for (i = 0; i < RAMPSIZE; i++) {
    GLfloat shade;
    shade = (GLfloat) i/(GLfloat) RAMPSIZE;
    auxSetOneColor (RAMPSTART+(GLint)i, shade, shade, shade);
    }
    glEnable (GL_POINT_SMOOTH);
    glHint (GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glPointSize (3.0);
    glClearIndex ((GLfloat) RAMPSTART);
}

/*  display() draws several points.  */

void CALLBACK display(void)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT);
    glIndexi (RAMPSTART);
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
    glOrtho (0.0, 100.0, 0.0, 
        100.0*(GLfloat) h/(GLfloat) w, -1.0, 1.0);
    else
    glOrtho (0.0, 100.0*(GLfloat) w/(GLfloat) h, 
        0.0, 100.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  color index display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_INDEX);
    auxInitPosition (0, 0, 100, 100);
    auxInitWindow ("Points");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
