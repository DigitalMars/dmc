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
 *  sccolorlight.c
 *  This program demonstrates the use of a colored 
 *  (magenta, in this example) light source.  Objects 
 *  are drawn using a grey material characteristic.  
 *  A single light source illuminates the objects.
 */
#include "glos.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

/*  Initialize material property and light source.
 */
void myinit(void)
{
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 0.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 0.0, 1.0, 1.0 };
/*  light_position is NOT default value */
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
}

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix ();
    glRotatef (20.0, 1.0, 0.0, 0.0);

    glPushMatrix ();
    glTranslatef (-0.75, 0.5, 0.0); 
    glRotatef (90.0, 1.0, 0.0, 0.0);
    auxSolidTorus (0.275, 0.85);
    glPopMatrix ();

    glPushMatrix ();
    glTranslatef (-0.75, -0.5, 0.0); 
    glRotatef (270.0, 1.0, 0.0, 0.0);
    auxSolidCone (1.0, 2.0);
    glPopMatrix ();

    glPushMatrix ();
    glTranslatef (0.75, 0.0, -1.0); 
    auxSolidSphere (1.0);
    glPopMatrix ();

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
    glOrtho (-2.5, 2.5, -2.5*(GLfloat)h/(GLfloat)w, 
        2.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else 
    glOrtho (-2.5*(GLfloat)w/(GLfloat)h, 
        2.5*(GLfloat)w/(GLfloat)h, -2.5, 2.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Colored Light Source");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
