/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *               1993-1995 Microsoft Corporation
 *
 * ALL RIGHTS RESERVED
 *
 * Please refer to OpenGL/readme.txt for additional information
 *
 */

/*  texturesurf.c
 *  This program uses evaluators to generate a curved
 *  surface and automatically generated texture coordinates.
 */
#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <math.h>

void myinit(void);
void makeImage(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

GLfloat ctrlpoints[4][4][3] = {
    {{ -1.5, -1.5, 4.0}, { -0.5, -1.5, 2.0}, 
    {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}}, 
    {{ -1.5, -0.5, 1.0}, { -0.5, -0.5, 3.0}, 
    {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}}, 
    {{ -1.5, 0.5, 4.0}, { -0.5, 0.5, 0.0}, 
    {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}}, 
    {{ -1.5, 1.5, -2.0}, { -0.5, 1.5, -2.0}, 
    {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};

GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}}, 
            {{1.0, 0.0}, {1.0, 1.0}}};

void CALLBACK display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    glFlush();
}

#define imageWidth 64
#define imageHeight 64
GLubyte image[3*imageWidth*imageHeight];

void makeImage(void)
{
    int i, j;
    float ti, tj;
    
    for (i = 0; i < imageWidth; i++) {
    ti = 2.0*3.14159265*i/imageWidth;
    for (j = 0; j < imageHeight; j++) {
        tj = 2.0*3.14159265*j/imageHeight;

        image[3*(imageHeight*i+j)] = (GLubyte) 127*(1.0+sin(ti));
        image[3*(imageHeight*i+j)+1] = (GLubyte) 127*(1.0+cos(2*tj));
        image[3*(imageHeight*i+j)+2] = (GLubyte) 127*(1.0+cos(ti+tj));
    }
    }
}

void myinit(void)
{
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
        0, 1, 12, 4, &ctrlpoints[0][0][0]);
    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 
        0, 1, 4, 2, &texpts[0][0][0]);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    makeImage();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth, imageHeight, 0,
         GL_RGB, GL_UNSIGNED_BYTE, image);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel (GL_FLAT);
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
    if (!h) return;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
    glOrtho(-4.0, 4.0, -4.0*(GLfloat)h/(GLfloat)w, 
        4.0*(GLfloat)h/(GLfloat)w, -4.0, 4.0);
    else
    glOrtho(-4.0*(GLfloat)w/(GLfloat)h, 
        4.0*(GLfloat)w/(GLfloat)h, -4.0, 4.0, -4.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(85.0, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv)
{
    auxInitDisplayMode (AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
    auxInitPosition (0, 0, 300, 300);
    auxInitWindow ("Evaluators");
    myinit();
    auxReshapeFunc (myReshape);
    auxMainLoop(display);
    return(0);
}
