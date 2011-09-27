/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *               1993-1995 Microsoft Corporation
 *
 * ALL RIGHTS RESERVED
 *
 * Please refer to OpenGL/readme.txt for additional information
 *
 */

#include "glos.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gl\glaux.h>

static void Build_lists(void );
static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Key_UP(void );
static void CALLBACK Key_DOWN(void );
static void CALLBACK Key_LEFT(void );
static void CALLBACK Key_RIGHT(void );
static void CALLBACK Key_D(void );
static void CALLBACK Key_d(void );
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum rgb, doubleBuffer;

double plane[4] = {
    1.0, 0.0, -1.0, 0.0
};
float rotX = 5.0, rotY = -5.0, zTranslate = -65.0;
float fogDensity = 0.02;
GLint cubeList = 1;

float scp[18][3] = {
    {
    1.000000, 0.000000, 0.000000
    },  
    {
    1.000000, 0.000000, 5.000000
    },
    {
    0.707107, 0.707107, 0.000000
    },  
    {
    0.707107, 0.707107, 5.000000
    },
    {
    0.000000, 1.000000, 0.000000
    },  
    {
    0.000000, 1.000000, 5.000000
    },
    {
    -0.707107, 0.707107, 0.000000
    },  
    {
    -0.707107, 0.707107, 5.000000
    },
    {
    -1.000000, 0.000000, 0.000000
    },  
    {
    -1.000000, 0.000000, 5.000000
    },
    {
    -0.707107, -0.707107, 0.000000
    },  
    {
    -0.707107, -0.707107, 5.000000
    },
    {
    0.000000, -1.000000, 0.000000
    },  
    {
    0.000000, -1.000000, 5.000000
    },
    {
    0.707107, -0.707107, 0.000000
    },  
    {
    0.707107, -0.707107, 5.000000
    },
    {
    1.000000, 0.000000, 0.000000
    },  
    {
    1.000000, 0.000000, 5.000000
    },
};


static void Build_lists(void)
{

    glNewList(cubeList, GL_COMPILE);
    glBegin(GL_TRIANGLE_STRIP);
       glNormal3fv(scp[0]); glVertex3fv(scp[0]);
       glNormal3fv(scp[0]); glVertex3fv(scp[1]);
       glNormal3fv(scp[2]); glVertex3fv(scp[2]);
       glNormal3fv(scp[2]); glVertex3fv(scp[3]);
       glNormal3fv(scp[4]); glVertex3fv(scp[4]);
       glNormal3fv(scp[4]); glVertex3fv(scp[5]);
       glNormal3fv(scp[6]); glVertex3fv(scp[6]);
       glNormal3fv(scp[6]); glVertex3fv(scp[7]);
       glNormal3fv(scp[8]); glVertex3fv(scp[8]);
       glNormal3fv(scp[8]); glVertex3fv(scp[9]);
       glNormal3fv(scp[10]); glVertex3fv(scp[10]);
       glNormal3fv(scp[10]); glVertex3fv(scp[11]);
       glNormal3fv(scp[12]); glVertex3fv(scp[12]);
       glNormal3fv(scp[12]); glVertex3fv(scp[13]);
       glNormal3fv(scp[14]); glVertex3fv(scp[14]);
       glNormal3fv(scp[14]); glVertex3fv(scp[15]);
       glNormal3fv(scp[16]); glVertex3fv(scp[16]);
       glNormal3fv(scp[16]); glVertex3fv(scp[17]);
    glEnd();
    glEndList();
}

static void Init(void)
{
    static float ambient[] = {0.1, 0.1, 0.1, 1.0};
    static float diffuse[] = {1.0, 1.0, 1.0, 1.0};
    static float position[] = {90.0, 90.0, 0.0, 0.0};
    static float front_mat_shininess[] = {30.0};
    static float front_mat_specular[] = {0.0, 0.0, 0.0, 1.0};
    static float front_mat_diffuse[] = {0.0, 1.0, 0.0, 1.0};
    static float back_mat_shininess[] = {50.0};
    static float back_mat_specular[] = {0.0, 0.0, 1.0, 1.0};
    static float back_mat_diffuse[] = {1.0, 0.0, 0.0, 1.0};
    static float lmodel_ambient[] = {0.0, 0.0, 0.0, 1.0};
    static float fog_color[] = {0.8, 0.8, 0.8, 1.0};
    
    glFrontFace(GL_CW);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);
    glMaterialfv(GL_BACK, GL_SHININESS, back_mat_shininess);
    glMaterialfv(GL_BACK, GL_SPECULAR, back_mat_specular);
    glMaterialfv(GL_BACK, GL_DIFFUSE, back_mat_diffuse);

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, fogDensity);
    if (rgb) {
    glFogfv(GL_FOG_COLOR, fog_color);
    glClearColor(0.8, 0.8, 0.8, 1.0);
    } else {
    glFogi(GL_FOG_INDEX, 1<<5);
    auxSetFogRamp(5, 3);
    glClearIndex(128);
    }

    Build_lists();
}

static void CALLBACK Reshape(int width, int height)
{

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

static void CALLBACK Key_UP(void)
{
    rotX -= 5;
}

static void CALLBACK Key_DOWN(void)
{
    rotX += 5;
}

static void CALLBACK Key_LEFT(void)
{
    rotY -= 5;
}

static void CALLBACK Key_RIGHT(void)
{
    rotY += 5;
}

static void CALLBACK Key_D(void)
{
    if (rgb) {
        fogDensity *= 1.10;
        glFogf(GL_FOG_DENSITY, fogDensity);
    }
}

static void CALLBACK Key_d(void)
{
    if (rgb) {
        fogDensity /= 1.10;
        glFogf(GL_FOG_DENSITY, fogDensity);
    }
}

static void CALLBACK Draw(void)
{

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glTranslatef(0, 0, zTranslate);
    glRotatef(rotY, 0,1,0);
    glRotatef(rotX, 1,0,0);
    glScalef(1.0, 1.0, 10.0);

    glCallList(cubeList);

    glPopMatrix();

    glFlush();

    if (doubleBuffer) {
    auxSwapBuffers();
    }
}

static GLenum Args(int argc, char **argv)
{
    GLint i;

    rgb = GL_TRUE;
    doubleBuffer = GL_FALSE;


    for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-ci") == 0) {
        rgb = GL_FALSE;
    } else if (strcmp(argv[i], "-rgb") == 0) {
        rgb = GL_TRUE;
    } else if (strcmp(argv[i], "-sb") == 0) {
        doubleBuffer = GL_FALSE;
    } else if (strcmp(argv[i], "-db") == 0) {
        doubleBuffer = GL_TRUE;
    } else {
        //printf("%s (Bad option).\n", argv[i]);
        return GL_FALSE;
    }
    }
    return GL_TRUE;
}

void main(int argc, char **argv)
{
    GLenum type;

    if (Args(argc, argv) == GL_FALSE) {
    auxQuit();
    }

    auxInitPosition(0, 0, 300, 300);

    type = AUX_DEPTH16;
    type |= (rgb) ? AUX_RGB : AUX_INDEX;
    type |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;

    auxInitDisplayMode(type);

    if (auxInitWindow("Fog Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_UP, Key_UP);
    auxKeyFunc(AUX_DOWN, Key_DOWN);
    auxKeyFunc(AUX_LEFT, Key_LEFT);
    auxKeyFunc(AUX_RIGHT, Key_RIGHT);
    auxKeyFunc(AUX_D, Key_D);
    auxKeyFunc(AUX_d, Key_d);
    auxMainLoop(Draw);
}
