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
 *  model.c
 *  This program demonstrates the use of OpenGL modeling 
 *  transformations.  Four triangles are drawn, each with 
 *  a different transformation.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void draw_triangle(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

void draw_triangle(void)
{
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.0, 25.0);
    glVertex2f(25.0, -25.0);
    glVertex2f(-25.0, -25.0);
    glEnd();
}

/*  Clear the screen.  For each triangle, set the current 
 *  color and modify the modelview matrix.
 */
void CALLBACK display(void)
{
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glClear (GL_COLOR_BUFFER_BIT);

    glLoadIdentity ();
    glColor3f (1.0, 1.0, 1.0);
    draw_triangle ();

    glEnable (GL_LINE_STIPPLE);
    glLineStipple (1, 0xF0F0);
    glLoadIdentity ();
    glTranslatef (-20.0, 0.0, 0.0);
    draw_triangle ();

    glLineStipple (1, 0xF00F);
    glLoadIdentity ();
    glScalef (1.5, 0.5, 1.0);
    draw_triangle ();

    glLineStipple (1, 0x8888);
    glLoadIdentity ();
    glRotatef (90.0, 0.0, 0.0, 1.0);
    draw_triangle ();
    glDisable (GL_LINE_STIPPLE);

    glFlush();
}

void myinit (void)
{
    glShadeModel (GL_FLAT);
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) 
    glOrtho (-50.0, 50.0, -50.0*(GLfloat)h/(GLfloat)w, 
        50.0*(GLfloat)h/(GLfloat)w, -1.0, 1.0);
    else 
    glOrtho (-50.0*(GLfloat)w/(GLfloat)h, 
        50.0*(GLfloat)w/(GLfloat)h, -50.0, 50.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Modeling Transformations");
    myinit ();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
