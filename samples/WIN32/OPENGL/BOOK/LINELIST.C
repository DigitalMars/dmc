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
 *  linelist.c  
 *  This program demonstrates using display lists to call 
 *  different line stipples.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK display(void);

#define drawOneLine(x1,y1,x2,y2) glBegin(GL_LINES); \
    glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2)); glEnd();

GLuint offset;

void myinit (void)
{
/*  background to be cleared to black   */
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);

    offset = glGenLists (3);
    glNewList (offset, GL_COMPILE);
    glDisable (GL_LINE_STIPPLE);
    glEndList ();
    glNewList (offset+1, GL_COMPILE);
    glEnable (GL_LINE_STIPPLE);
    glLineStipple (1, 0x0F0F);
    glEndList ();
    glNewList (offset+2, GL_COMPILE);
    glEnable (GL_LINE_STIPPLE);
    glLineStipple (1, 0x1111);
    glEndList ();
}

void CALLBACK display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);

/*  draw all lines in white */
    glColor3f (1.0, 1.0, 1.0);

    glCallList (offset);
    drawOneLine (50.0, 125.0, 350.0, 125.0);
    glCallList (offset+1);
    drawOneLine (50.0, 100.0, 350.0, 100.0);
    glCallList (offset+2);
    drawOneLine (50.0, 75.0, 350.0, 75.0);
    glCallList (offset+1);
    drawOneLine (50.0, 50.0, 350.0, 50.0);
    glCallList (offset);
    drawOneLine (50.0, 25.0, 350.0, 25.0);
    glFlush ();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 400, 150);
    auxInitWindow ("Display Lists");
    myinit ();
    auxMainLoop(display);
    return(0);
}
