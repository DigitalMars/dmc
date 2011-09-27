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
 *  pickline.c
 *  Picking is demonstrated here.  Press the left mouse button
 *  is enter picking mode.  You get two hits if you press the
 *  mouse, while the cursor is where the lines intersect.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <stdio.h>

void myinit(void);
void drawLine(GLenum mode);
void CALLBACK pickLine(AUX_EVENTREC *event);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

void drawLine(GLenum mode)
{
    if (mode == GL_SELECT)
    glLoadName (1);
    glBegin (GL_LINES);
    glColor3f (1.0, 1.0, 1.0);
    glVertex3f (30.0, 30.0, 0.0);
    glVertex3f (50.0, 60.0, 0.0);
    glEnd (); 

    if (mode == GL_SELECT)
    glLoadName (2);
    glBegin (GL_LINES);
    glColor3f (1.0, 1.0, 1.0);
    glVertex3f (50.0, 60.0, 0.0);
    glVertex3f (70.0, 40.0, 0.0);
    glEnd (); 
}

/*  pickline() is called when the mouse is pressed.
 *  The projection matrix is reloaded to include the
 *  picking matrix.  The line is "redrawn" during
 *  selection mode, and names are sent to the buffer.
 */
#define BUFSIZE 512

void CALLBACK pickLine(AUX_EVENTREC *event)
{
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];
    int x, y;

    x = event->data[AUX_MOUSEX];
    y = event->data[AUX_MOUSEY];
    glGetIntegerv (GL_VIEWPORT, viewport);

    glSelectBuffer (BUFSIZE, selectBuf);
    (void) glRenderMode (GL_SELECT);

    glInitNames();
    glPushName((GLuint)-1);

    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
    gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 
    5.0, 5.0, viewport);
    gluOrtho2D (0.0, (GLdouble) viewport[2], 
    0.0, (GLdouble) viewport[3]);
    drawLine (GL_SELECT);
    glPopMatrix ();
    glFlush ();

    hits = glRenderMode (GL_RENDER);
    //printf ("hits is %d\n", hits);
} 

void myinit (void) 
{
    glShadeModel (GL_FLAT);
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawLine (GL_RENDER);
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0.0, (GLdouble) w, 0.0, (GLdouble) h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 100, 100);
    auxInitWindow ("Picking Lines");
    myinit ();
    auxMouseFunc (AUX_LEFTBUTTON, AUX_MOUSEDOWN, pickLine);
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
