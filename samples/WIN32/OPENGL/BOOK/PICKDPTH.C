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
 *  pickdepth.c
 *  Picking is demonstrated in this program.  In 
 *  rendering mode, three overlapping rectangles are 
 *  drawn.  When the left mouse button is pressed, 
 *  selection mode is entered with the picking matrix.  
 *  Rectangles which are drawn under the cursor position
 *  are "picked."  Pay special attention to the depth 
 *  value range, which is returned.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <stdio.h>

void myinit(void);
void drawRects(GLenum mode);
void processHits (GLint hits, GLuint buffer[]);
void CALLBACK pickRects(AUX_EVENTREC *event);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

void myinit(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glDepthRange (0.0, 1.0);    /*  The default z mapping   */
}

/*  The three rectangles are drawn.  In selection mode, 
 *  each rectangle is given the same name.  Note that 
 *  each rectangle is drawn with a different z value.
 */
void drawRects(GLenum mode)
{
    if (mode == GL_SELECT)
    glLoadName (1);
    glBegin (GL_QUADS);
    glColor3f (1.0, 1.0, 0.0);
    glVertex3i (2, 0, 0);
    glVertex3i (2, 6, 0);
    glVertex3i (6, 6, 0);
    glVertex3i (6, 0, 0);
    glColor3f (0.0, 1.0, 1.0);
    glVertex3i (3, 2, -1);
    glVertex3i (3, 8, -1);
    glVertex3i (8, 8, -1);
    glVertex3i (8, 2, -1);
    glColor3f (1.0, 0.0, 1.0);
    glVertex3i (0, 2, -2);
    glVertex3i (0, 7, -2);
    glVertex3i (5, 7, -2);
    glVertex3i (5, 2, -2);
    glEnd ();
}

/*  processHits() prints out the contents of the 
 *  selection array.
 */
void processHits (GLint hits, GLuint buffer[])
{
    int i;
    GLint names, *ptr;

    //printf ("hits = %d\n", hits);
    ptr = (GLuint *) buffer;
    for (i = 0; i < hits; i++) {    /*  for each hit  */
        names = *ptr;
        //printf (" number of names for hit = %d\n", names); ptr++;
        //printf ("  z1 is %u;", *ptr); ptr++;
        //printf (" z2 is %u\n", *ptr); ptr++;
        //printf ("   the name is ");
/*
        for (j = 0; j < names; j++) {
            printf ("%d ", *ptr); ptr++;
        }
*/
        //printf ("\n");
    }
}

/*  pickRects() sets up selection mode, name stack, 
 *  and projection matrix for picking.  Then the objects 
 *  are drawn.
 */
#define BUFSIZE 512

void CALLBACK pickRects(AUX_EVENTREC *event)
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
    glPushName((unsigned)-1);

    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
/*  create 5x5 pixel picking region near cursor location    */
    gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 
    5.0, 5.0, viewport);
    glOrtho (0.0, 8.0, 0.0, 8.0, -0.5, 2.5);
    drawRects (GL_SELECT);
    glPopMatrix ();
    glFlush ();

    hits = glRenderMode (GL_RENDER);
    //processHits (hits, selectBuf);
} 

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawRects (GL_RENDER);
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0.0, 8.0, 0.0, 8.0, 0.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, depth buffer, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 100, 100);
    auxInitWindow ("Picking Rectangles");
    myinit ();
    auxMouseFunc (AUX_LEFTBUTTON, AUX_MOUSEDOWN, pickRects);
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
