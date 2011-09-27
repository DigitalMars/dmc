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


#define PI 3.141592654
#define BLACK 0
#define GRAY 128
#define WHITE 255
#define RD 0xA40000FF
#define WT 0xFFFFFFFF
#define brickImageWidth 16
#define brickImageHeight 16

static void CALLBACK ErrorHandler(GLenum which);
static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Key_LEFT(void );
static void CALLBACK Key_RIGHT(void );
static void CALLBACK Key_UP(void );
static void CALLBACK Key_DOWN(void );
static void CALLBACK Key_X(void );
static void CALLBACK Key_x(void );
static void CALLBACK Key_1(void );
static void CALLBACK Key_2(void );
static void CALLBACK Key_3(void );
static void CALLBACK Key_4(void );
static void CALLBACK Key_0(void );
static void CALLBACK Key_A(void );
static void CALLBACK Key_a(void );
static void CALLBACK Key_S(void );
static void CALLBACK Key_s(void );
static void CALLBACK Key_d(void );
static void CALLBACK Key_f(void );
static void CALLBACK Key_G(void );
static void CALLBACK Key_g(void );
static void CALLBACK Key_J(void );
static void CALLBACK Key_j(void );
static void CALLBACK Key_H(void );
static void CALLBACK Key_h(void );
static void CALLBACK Key_K(void );
static void CALLBACK Key_k(void );
static void CALLBACK Key_L(void );
static void CALLBACK Key_l(void );
static void CALLBACK Key_z(void );
static void CALLBACK Key_q(void );
static void CALLBACK Key_w(void );
static void CALLBACK Key_e(void );
static void CALLBACK Key_r(void );
static void CALLBACK Key_t(void );
static void CALLBACK Key_y(void );
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum rgb, doubleBuffer;

float black[3] = {
    0.0, 0.0, 0.0
};
float blue[3] =  {
    0.0, 0.0, 1.0
};
float gray[3] =  {
    0.5, 0.5, 0.5
};
float white[3] = {
    1.0, 1.0, 1.0
};

GLenum doDither = GL_TRUE;
GLenum shade = GL_TRUE;
GLenum texture = GL_TRUE;

float xRotation = 30.0, yRotation = 30.0, zRotation = 0.0;
GLint radius1, radius2;
GLdouble angle1, angle2;
GLint slices, stacks;
GLint height;
GLint orientation = GLU_OUTSIDE;
GLint whichQuadric;
GLUquadricObj *quadObj;

GLuint brickImage[brickImageWidth*brickImageHeight] = {
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD, RD,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    RD, RD, RD, RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD,
    WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT, WT,
    RD, RD, RD, RD, WT, RD, RD, RD, RD, RD, RD, RD, RD, RD, WT, RD
};
char *texFileName = 0;


static void CALLBACK ErrorHandler(GLenum which)
{

    fprintf(stderr, "Quad Error: %s\n", gluErrorString(which));
}

static void Init(void)
{
    static GLint colorIndexes[3] = {0, 200, 255};
    static float ambient[] = {0.1, 0.1, 0.1, 1.0};
    static float diffuse[] = {0.5, 1.0, 1.0, 1.0};
    static float position[] = {90.0, 90.0, 150.0, 0.0};
    static float front_mat_shininess[] = {30.0};
    static float front_mat_specular[] = {0.2, 0.2, 0.2, 1.0};
    static float front_mat_diffuse[] = {0.5, 0.28, 0.38, 1.0};
    static float back_mat_shininess[] = {50.0};
    static float back_mat_specular[] = {0.5, 0.5, 0.2, 1.0};
    static float back_mat_diffuse[] = {1.0, 1.0, 0.2, 1.0};
    static float lmodel_ambient[] = {1.0, 1.0, 1.0, 1.0};
    static float lmodel_twoside[] = {GL_TRUE};
    static float decal[] = {GL_DECAL};
    static float modulate[] = {GL_MODULATE};
    static float repeat[] = {GL_REPEAT};
    static float nearest[] = {GL_NEAREST};
    AUX_RGBImageRec *image;

    if (!rgb) {
    auxSetGreyRamp();
    }
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMaterialfv(GL_FRONT, GL_SHININESS, front_mat_shininess);
    glMaterialfv(GL_FRONT, GL_SPECULAR, front_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, front_mat_diffuse);
    glMaterialfv(GL_BACK, GL_SHININESS, back_mat_shininess);
    glMaterialfv(GL_BACK, GL_SPECULAR, back_mat_specular);
    glMaterialfv(GL_BACK, GL_DIFFUSE, back_mat_diffuse);
    if (!rgb) {
    glMaterialiv( GL_FRONT_AND_BACK, GL_COLOR_INDEXES, colorIndexes);
    }

    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, decal);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nearest);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nearest);
    if (texFileName) {
    image = auxRGBImageLoad(texFileName);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->sizeX, image->sizeY,
              GL_RGB, GL_UNSIGNED_BYTE, image->data);
    } else {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, brickImageWidth, brickImageHeight,
             0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)brickImage);
    }

    quadObj = gluNewQuadric();
    //gluQuadricCallback(quadObj, GLU_ERROR, ErrorHandler);

    radius1 = 10;
    radius2 = 5;
    angle1 = 90;
    angle2 = 180;
    slices = 16;
    stacks = 10;
    height = 20;
}

static void CALLBACK Reshape(int width, int height)
{

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 1, 10);
    gluLookAt(2, 2, 2, 0, 0, 0, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
}

static void CALLBACK Key_LEFT(void)
{
    yRotation += 5;
}

static void CALLBACK Key_RIGHT(void)
{
    yRotation -= 5;
}

static void CALLBACK Key_UP(void)
{
    xRotation += 5;
}

static void CALLBACK Key_DOWN(void)
{
    xRotation -= 5;
}

static void CALLBACK Key_X(void)
{
    zRotation += 5;
}

static void CALLBACK Key_x(void)
{
    zRotation -= 5;
}

static void CALLBACK Key_1(void)
{
    gluQuadricDrawStyle(quadObj, GLU_FILL);
}

static void CALLBACK Key_2(void)
{
    gluQuadricDrawStyle(quadObj, GLU_POINT);
}

static void CALLBACK Key_3(void)
{
    gluQuadricDrawStyle(quadObj, GLU_LINE);
}

static void CALLBACK Key_4(void)
{
    gluQuadricDrawStyle(quadObj, GLU_SILHOUETTE);
}

static void CALLBACK Key_0(void)
{
    shade = !shade;
    if (shade) {
        glShadeModel(GL_SMOOTH);
        gluQuadricNormals(quadObj, GLU_SMOOTH);
    } else {
        glShadeModel(GL_FLAT);
        gluQuadricNormals(quadObj, GLU_FLAT);
    }
}

static void CALLBACK Key_A(void)
{
    stacks++;
}

static void CALLBACK Key_a(void)
{
    if (stacks > 1)
        stacks--;
}

static void CALLBACK Key_S(void)
{
    slices++;
}

static void CALLBACK Key_s(void)
{
    if (slices > 3)
        slices--;
}

static void CALLBACK Key_d(void)
{
    switch(orientation) {
      case GLU_OUTSIDE:
        orientation = GLU_INSIDE;
        break;
      case GLU_INSIDE:
      default:
        orientation = GLU_OUTSIDE;
        break;
    }
    gluQuadricOrientation(quadObj, orientation);
}

static void CALLBACK Key_f(void)
{
    whichQuadric = (whichQuadric + 1) & 0x03;
}

static void CALLBACK Key_G(void)
{
    radius1 += 1;
}

static void CALLBACK Key_g(void)
{
    if (radius1 > 0)
        radius1 -= 1;
}

static void CALLBACK Key_J(void)
{
    radius2 += 1;
}

static void CALLBACK Key_j(void)
{
    if (radius2 > 0)
        radius2 -= 1;
}

static void CALLBACK Key_H(void)
{
    height += 2;
}

static void CALLBACK Key_h(void)
{
    if (height > 0)
        height -= 2;
}

static void CALLBACK Key_K(void)
{
    angle1 += 5;
}

static void CALLBACK Key_k(void)
{
    angle1 -= 5;
}

static void CALLBACK Key_L(void)
{
    angle2 += 5;
}

static void CALLBACK Key_l(void)
{
    angle2 -= 5;
}

static void CALLBACK Key_z(void)
{
        texture = !texture;
    if (texture) {
        gluQuadricTexture(quadObj, GL_TRUE);
        glEnable(GL_TEXTURE_2D);
    } else {
        gluQuadricTexture(quadObj, GL_FALSE);
        glDisable(GL_TEXTURE_2D);
    }
}

static void CALLBACK Key_q(void)
{
    glDisable(GL_CULL_FACE);
}

static void CALLBACK Key_w(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
}

static void CALLBACK Key_e(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

static void CALLBACK Key_r(void)
{
    glFrontFace(GL_CW);
}

static void CALLBACK Key_t(void)
{
    glFrontFace(GL_CCW);
}

static void CALLBACK Key_y(void)
{
    doDither = !doDither;
    (doDither) ? glEnable(GL_DITHER) : glDisable(GL_DITHER);
}

static void CALLBACK Draw(void)
{

    glLoadIdentity();
    glRotatef(xRotation, 1, 0, 0);
    glRotatef(yRotation, 0, 1, 0);
    glRotatef(zRotation, 0, 0, 1);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    switch (whichQuadric) {
      case 0:
    glTranslatef(0, 0, -height/20.0);
    gluCylinder(quadObj, radius1/10.0, radius2/10.0, height/10.0, 
            slices, stacks);
    break;
      case 1:
    gluSphere(quadObj, radius1/10.0, slices, stacks);
    break;
      case 2:
    gluPartialDisk(quadObj, radius2/10.0, radius1/10.0, slices, 
               stacks, angle1, angle2);
    break;
      case 3:
    gluDisk(quadObj, radius2/10.0, radius1/10.0, slices, stacks);
    break;
    }

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
    } else if (strcmp(argv[i], "-f") == 0) {
        if (i+1 >= argc || argv[i+1][0] == '-') {
        //printf("-f (No file name).\n");
        return GL_FALSE;
        } else {
        texFileName = argv[++i];
        }
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

    if (auxInitWindow("Quad Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_LEFT, Key_LEFT);
    auxKeyFunc(AUX_RIGHT, Key_RIGHT);
    auxKeyFunc(AUX_UP, Key_UP);
    auxKeyFunc(AUX_DOWN, Key_DOWN);
    auxKeyFunc(AUX_X, Key_X);
    auxKeyFunc(AUX_x, Key_x);
    auxKeyFunc(AUX_1, Key_1);
    auxKeyFunc(AUX_2, Key_2);
    auxKeyFunc(AUX_3, Key_3);
    auxKeyFunc(AUX_4, Key_4);
    auxKeyFunc(AUX_0, Key_0);
    auxKeyFunc(AUX_A, Key_A);
    auxKeyFunc(AUX_a, Key_a);
    auxKeyFunc(AUX_S, Key_S);
    auxKeyFunc(AUX_s, Key_s);
    auxKeyFunc(AUX_d, Key_d);
    auxKeyFunc(AUX_f, Key_f);
    auxKeyFunc(AUX_G, Key_G);
    auxKeyFunc(AUX_g, Key_g);
    auxKeyFunc(AUX_J, Key_J);
    auxKeyFunc(AUX_j, Key_j);
    auxKeyFunc(AUX_H, Key_H);
    auxKeyFunc(AUX_h, Key_h);
    auxKeyFunc(AUX_K, Key_K);
    auxKeyFunc(AUX_k, Key_k);
    auxKeyFunc(AUX_L, Key_L);
    auxKeyFunc(AUX_l, Key_l);
    auxKeyFunc(AUX_z, Key_z);
    auxKeyFunc(AUX_Q, Key_q);
    auxKeyFunc(AUX_w, Key_w);
    auxKeyFunc(AUX_e, Key_e);
    auxKeyFunc(AUX_r, Key_r);
    auxKeyFunc(AUX_t, Key_t);
    auxKeyFunc(AUX_y, Key_y);
    auxMainLoop(Draw);
}
