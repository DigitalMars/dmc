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
 *  cube.c
 *  Draws a 3-D cube, viewed with perspective, stretched 
 *  along the y-axis.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

/*  Clear the screen.  Set the current color to white.
 *  Draw the wire frame cube.
 */
void CALLBACK display (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    glLoadIdentity ();  /*  clear the matrix    */
    glTranslatef (0.0, 0.0, -5.0);  /*  viewing transformation  */
    glScalef (1.0, 2.0, 1.0);   /*  modeling transformation */
    auxWireCube(1.0);   /*  draw the cube   */
    glFlush();
}

void myinit (void) {
    glShadeModel (GL_FLAT);
}

/*  Called when the window is first opened and whenever 
 *  the window is reconfigured (moved or resized).
 */
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    glMatrixMode (GL_PROJECTION);   /*  prepare for and then  */ 
    glLoadIdentity ();  /*  define the projection  */
    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);    /*  transformation  */
    glMatrixMode (GL_MODELVIEW);    /*  back to modelview matrix    */
    glViewport (0, 0, w, h);    /*  define the viewport */
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Perspective 3-D Cube");
    myinit ();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
