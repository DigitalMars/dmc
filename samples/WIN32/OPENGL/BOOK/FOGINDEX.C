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
 *  fogindex.c
 *  This program demonstrates fog in color index mode.  
 *  Three cones are drawn at different z values in a linear 
 *  fog.  32 contiguous colors (from 16 to 47) are loaded 
 *  with a color ramp.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

/*  Initialize color map and fog.  Set screen clear color
 *  to end of color ramp.
 */
#define NMCOLORS 32
#define RAMPSTART 16

void myinit(void)
{
    int i;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    for (i = 0; i < NMCOLORS; i++) {
    GLfloat shade;
    shade = (GLfloat) (NMCOLORS-i)/(GLfloat) NMCOLORS;
    auxSetOneColor (16 + i, shade, shade, shade);
    }
    glEnable(GL_FOG);

    glFogi (GL_FOG_MODE, GL_LINEAR);
    glFogi (GL_FOG_INDEX, NMCOLORS);
    glFogf (GL_FOG_START, 0.0);
    glFogf (GL_FOG_END, 4.0);
    glHint (GL_FOG_HINT, GL_NICEST);
    glClearIndex((GLfloat) (NMCOLORS+RAMPSTART-1));
}

/*  display() renders 3 cones at different z positions.
 */
void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix ();
    glTranslatef (-1.0, -1.0, -1.0);
    glRotatef (-90.0, 1.0, 0.0, 0.0);
    glIndexi (RAMPSTART);
    auxSolidCone(1.0, 2.0);
    glPopMatrix ();

    glPushMatrix ();
    glTranslatef (0.0, -1.0, -2.25);
    glRotatef (-90.0, 1.0, 0.0, 0.0);
    glIndexi (RAMPSTART);
    auxSolidCone(1.0, 2.0);
    glPopMatrix ();

    glPushMatrix ();
    glTranslatef (1.0, -1.0, -3.5);
    glRotatef (-90.0, 1.0, 0.0, 0.0);
    glIndexi (RAMPSTART);
    auxSolidCone(1.0, 2.0);
    glPopMatrix ();
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
    glOrtho (-2.0, 2.0, -2.0*(GLfloat)h/(GLfloat)w, 
        2.0*(GLfloat)h/(GLfloat)w, 0.0, 10.0);
    else
    glOrtho (-2.0*(GLfloat)w/(GLfloat)h, 
        2.0*(GLfloat)w/(GLfloat)h, -2.0, 2.0, 0.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, depth buffer, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_INDEX | AUX_DEPTH16);
    auxInitPosition (0, 0, 200, 200);
    auxInitWindow ("Fog Using Color Index");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
