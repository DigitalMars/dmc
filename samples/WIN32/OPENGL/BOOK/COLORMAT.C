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
 *  colormat.c
 *  After initialization, the program will be in 
 *  ColorMaterial mode.  Pressing the mouse buttons 
 *  will change the color of the diffuse reflection.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

void myinit(void);
void CALLBACK changeRedDiffuse (void);
void CALLBACK changeGreenDiffuse (void);
void CALLBACK changeBlueDiffuse (void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);

GLfloat diffuseMaterial[4] = { 0.5, 0.5, 0.5, 1.0 };

/*  Initialize values for material property, light source, 
 *  lighting model, and depth buffer.  
 */
void myinit(void)
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void CALLBACK changeRedDiffuse (void)
{
    diffuseMaterial[0] += 0.1;
    if (diffuseMaterial[0] > 1.0)
    diffuseMaterial[0] = 0.0;
    glColor4fv(diffuseMaterial);
}

void CALLBACK changeGreenDiffuse (void)
{
    diffuseMaterial[1] += 0.1;
    if (diffuseMaterial[1] > 1.0)
    diffuseMaterial[1] = 0.0;
    glColor4fv(diffuseMaterial);
}

void CALLBACK changeBlueDiffuse (void)
{
    diffuseMaterial[2] += 0.1;
    if (diffuseMaterial[2] > 1.0)
    diffuseMaterial[2] = 0.0;
    glColor4fv(diffuseMaterial);
}

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auxSolidSphere(1.0);
    glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) 
    glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w, 
        1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
    else 
    glOrtho (-1.5*(GLfloat)w/(GLfloat)h, 
        1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 500, 500);
    auxInitWindow ("ColorMaterial Mode");
    myinit();
    auxKeyFunc(AUX_R, changeRedDiffuse);
    auxKeyFunc(AUX_r, changeRedDiffuse);
    auxKeyFunc(AUX_G, changeGreenDiffuse);
    auxKeyFunc(AUX_g, changeGreenDiffuse);
    auxKeyFunc(AUX_B, changeBlueDiffuse);
    auxKeyFunc(AUX_b, changeBlueDiffuse);
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
