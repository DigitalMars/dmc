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
 *  aim.c
 *  This program calculates the fovy (field of view angle
 *  in the y direction), by using trigonometry, given the
 *  size of an object and its size.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <GL/glaux.h>
#include <stdio.h>

void myinit(void);
GLdouble calculateAngle (double size, double distance);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

void myinit (void) {
    glShadeModel (GL_FLAT);
}

/*  Clear the screen.  Set the current color to white.
 *  Draw the wire frame cube and sphere.
 */
void CALLBACK display (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);

    glLoadIdentity ();
/*  glTranslatef() as viewing transformation    */
    glTranslatef (0.0, 0.0, -5.0);
    auxWireCube(2.0);
    auxWireSphere(1.0);
    glFlush();
}

#define PI  3.1415926535

/*  atan2 () is a system math routine which calculates
 *  the arctangent of an angle, given length of the 
 *  opposite and adjacent sides of a right triangle.
 *  atan2 () is not an OpenGL routine.
 */
GLdouble calculateAngle (double size, double distance)
{
    GLdouble radtheta, degtheta;
    
    radtheta = 2.0 * atan2 (size/2.0, distance);
    degtheta = (180.0 * radtheta) / PI;
    //printf ("degtheta is %lf\n", degtheta);
    return ((GLdouble) degtheta);
}

/*  Called when the window is first opened and whenever 
 *  the window is reconfigured (moved or resized).
 */
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    GLdouble theta;

    if(!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    theta = calculateAngle (2.0, 5.0);
    gluPerspective(theta, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
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
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Field of View Angle");
    myinit ();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
