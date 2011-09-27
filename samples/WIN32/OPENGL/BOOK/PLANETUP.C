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
 *  planetup.c
 *  The planets (from planet.c) have been rotated so their 
 *  polar regions are north/south.  
 *  Interaction:  pressing the left, right, up, and down 
 *  arrow keys alters the rotation of the planet around the sun.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void drawPlane(void);
void CALLBACK dayAdd (void);
void CALLBACK daySubtract (void);
void CALLBACK yearAdd (void);
void CALLBACK yearSubtract (void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

static int year = 0, day = 0;

void CALLBACK dayAdd (void)
{
    day = (day + 10) % 360;
}

void CALLBACK daySubtract (void)
{
    day = (day - 10) % 360;
}

void CALLBACK yearAdd (void)
{
    year = (year + 5) % 360;
}

void CALLBACK yearSubtract (void)
{
    year = (year - 5) % 360;
}

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f (1.0, 1.0, 1.0);
    glPushMatrix();
/*  draw sun    */
    glPushMatrix();
    glRotatef (90.0, 1.0, 0.0, 0.0);  /*  rotate it upright  */
    auxWireSphere(1.0);
    glPopMatrix();
/*  draw smaller planet */
    glRotatef ((GLfloat) year, 0.0, 1.0, 0.0);
    glTranslatef (2.0, 0.0, 0.0);
    glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
    glRotatef (90.0, 1.0, 0.0, 0.0);  /*  rotate it upright  */
    auxWireSphere(0.2);
    glPopMatrix();
    glFlush();
}

void myinit (void) {
    glShadeModel (GL_FLAT);
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef (0.0, 0.0, -5.0);
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Composite Modeling Transformations");
    myinit ();
    auxKeyFunc (AUX_LEFT, yearSubtract);
    auxKeyFunc (AUX_RIGHT, yearAdd);
    auxKeyFunc (AUX_UP, dayAdd);
    auxKeyFunc (AUX_DOWN, daySubtract);
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
