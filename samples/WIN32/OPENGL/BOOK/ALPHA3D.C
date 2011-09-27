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
 *  alpha3D.c
 *  This program demonstrates how to intermix opaque and
 *  alpha blended polygons in the same scene, by using glDepthMask.
 *  Pressing the left mouse button toggles the eye position.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK toggleEye (AUX_EVENTREC *event);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

void myinit(void)
{
    GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 0.15 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.15 };
    GLfloat mat_shininess[] = { 15.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
}

GLboolean eyePosition = GL_FALSE;

void CALLBACK toggleEye (AUX_EVENTREC *event)
{
    if (eyePosition)
    eyePosition = GL_FALSE;
    else
    eyePosition = GL_TRUE;
}

void CALLBACK display(void)
{
    GLfloat position[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat mat_torus[] = { 0.75, 0.75, 0.0, 1.0 };
    GLfloat mat_cylinder[] = { 0.0, 0.75, 0.75, 0.15 };

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLightfv (GL_LIGHT0, GL_POSITION, position);
    glPushMatrix ();
    if (eyePosition)
        gluLookAt (0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
    else 
        gluLookAt (0.0, 0.0, -9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
    glPushMatrix ();
    glTranslatef (0.0, 0.0, 1.0);    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_torus);
    auxSolidTorus (0.275, 0.85);
    glPopMatrix ();

    glEnable (GL_BLEND);
    glDepthMask (GL_FALSE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_cylinder);
    glTranslatef (0.0, 0.0, -1.0);    
    auxSolidCylinder (1.0, 2.0);
    glDepthMask (GL_TRUE);
    glDisable (GL_BLEND);
    glPopMatrix ();

    glFlush ();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if(!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("Intermixing Opaque and Alpha Blending");
    auxMouseFunc (AUX_LEFTBUTTON, AUX_MOUSEDOWN, toggleEye);
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
