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
#include <math.h>
#include <stdlib.h>
#include <gl\glaux.h>

static void BuildCube(void );
static void BuildLists(void );
static void Init(void );
static void CALLBACK Reshape(int width,int height);
static void CALLBACK Key_LEFT(void );
static void CALLBACK Key_RIGHT(void );
static void CALLBACK Key_UP(void );
static void CALLBACK Key_DOWN(void );
static void CALLBACK Key_T(void );
static void CALLBACK Key_t(void );
static void CALLBACK Key_s(void );
static void CALLBACK Key_0(void );
static void CALLBACK Key_1(void );
static void CALLBACK Key_2(void );
static void CALLBACK Key_3(void );
static void CALLBACK Key_4(void );
static void CALLBACK Key_5(void );
static void CALLBACK Key_6(void );
static void CALLBACK Key_7(void );
static void CALLBACK Draw(void );
static GLenum Args(int argc,char **argv );

GLenum doubleBuffer;

char *texFileName = 0;
AUX_RGBImageRec *image;

float *minFilter, *magFilter, *sWrapMode, *tWrapMode;
float decal[] = {GL_DECAL};
float modulate[] = {GL_MODULATE};
float repeat[] = {GL_REPEAT};
float clamp[] = {GL_CLAMP};
float nr[] = {GL_NEAREST};
float ln[] = {GL_LINEAR};
float nr_mipmap_nr[] = {GL_NEAREST_MIPMAP_NEAREST};
float nr_mipmap_ln[] = {GL_NEAREST_MIPMAP_LINEAR};
float ln_mipmap_nr[] = {GL_LINEAR_MIPMAP_NEAREST};
float ln_mipmap_ln[] = {GL_LINEAR_MIPMAP_LINEAR};
GLint sphereMap[] = {GL_SPHERE_MAP};

GLenum doSphere = GL_FALSE;
float xRotation = 0.0, yRotation = 0.0, zTranslate = -3.125;

GLint cube;
float c[6][4][3] = {
    {
    {
        1.0, 1.0, -1.0
    },
    {
        -1.0, 1.0, -1.0
    },
    {
        -1.0, -1.0, -1.0
    },
    {
        1.0, -1.0, -1.0
    }
    },
    {
    {
        1.0, 1.0, 1.0
    },
    {
        1.0, 1.0, -1.0
    },
    {
        1.0, -1.0, -1.0
    },
    {
        1.0, -1.0, 1.0
    }
    },
    {
    {
        -1.0, 1.0, 1.0
    },
    {
        1.0, 1.0, 1.0
    },
    {
        1.0, -1.0, 1.0
    },
    {
        -1.0, -1.0, 1.0
    }
    },
    {
    {
        -1.0, 1.0, -1.0
    },
    {
        -1.0, 1.0, 1.0
    },
    {
        -1.0, -1.0, 1.0
    },
    {
        -1.0, -1.0, -1.0
    }
    },
    {
    {
        -1.0, 1.0, 1.0
    },
    {
        -1.0, 1.0, -1.0
    },
    {
        1.0, 1.0, -1.0
    },
    {
        1.0, 1.0, 1.0
    }
    },
    {
    {
        -1.0, -1.0, -1.0
    },
    {
        -1.0, -1.0, 1.0
    },
    {
        1.0, -1.0, 1.0
    },
    {
        1.0, -1.0, -1.0
    }
    }
};
static float n[6][3] = {
    {
    0.0, 0.0, -1.0
    },
    {
    1.0, 0.0, 0.0
    },
    {
    0.0, 0.0, 1.0
    },
    {
    -1.0, 0.0, 0.0
    },
    {
    0.0, 1.0, 0.0
    },
    {
    0.0, -1.0, 0.0
    }
};
static float t[6][4][2] = {
    {
    {
        1.1,  1.1
    },
    {
        -0.1, 1.1
    },
    {
        -0.1, -0.1
    },
    {
        1.1,  -0.1
    }
    },
    {
    {
        1.1,  1.1
    },
    {
        -0.1, 1.1
    },
    {
        -0.1, -0.1
    },
    {
        1.1,  -0.1
    }
    },
    {
    {
        -0.1,  1.1
    },
    {
        1.1, 1.1
    },
    {
        1.1, -0.1
    },
    {
        -0.1,  -0.1
    }
    },
    {
    {
        1.1,  1.1
    },
    {
        -0.1, 1.1
    },
    {
        -0.1, -0.1
    },
    {
        1.1,  -0.1
    }
    },
    {
    {
        1.1,  1.1
    },
    {
        -0.1, 1.1
    },
    {
        -0.1, -0.1
    },
    {
        1.1,  -0.1
    }
    },
    {
    {
        1.1,  1.1
    },
    {
        -0.1, 1.1
    },
    {
        -0.1, -0.1
    },
    {
        1.1,  -0.1
    }
    },
};


static void BuildCube(void)
{
    GLint i;

    glNewList(cube, GL_COMPILE);
    for (i = 0; i < 6; i++) {
    glBegin(GL_POLYGON);
        glNormal3fv(n[i]); glTexCoord2fv(t[i][0]); glVertex3fv(c[i][0]);
        glNormal3fv(n[i]); glTexCoord2fv(t[i][1]); glVertex3fv(c[i][1]);
        glNormal3fv(n[i]); glTexCoord2fv(t[i][2]); glVertex3fv(c[i][2]);
        glNormal3fv(n[i]); glTexCoord2fv(t[i][3]); glVertex3fv(c[i][3]);
    glEnd();
    }
    glEndList();
}

static void BuildLists(void)
{

    cube = glGenLists(1);
    BuildCube();
}

static void Init(void)
{

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->sizeX, image->sizeY,
              GL_RGB, GL_UNSIGNED_BYTE, image->data);
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, decal);
    glEnable(GL_TEXTURE_2D);

    glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);

    BuildLists();

    glClearColor(0.0, 0.0, 0.0, 0.0);

    magFilter = nr;
    minFilter = nr;
    sWrapMode = repeat;
    tWrapMode = repeat;
}

static void CALLBACK Reshape(int width, int height)
{

    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(145.0, 1.0, 0.01, 1000);
    glMatrixMode(GL_MODELVIEW);
}

static void CALLBACK Key_LEFT(void)
{
    yRotation -= 0.5;
}

static void CALLBACK Key_RIGHT(void)
{
    yRotation += 0.5;
}

static void CALLBACK Key_UP(void)
{
    xRotation -= 0.5;
}

static void CALLBACK Key_DOWN(void)
{
    xRotation += 0.5;
}

static void CALLBACK Key_T(void)
{
    zTranslate += 0.25;
}

static void CALLBACK Key_t(void)
{
    zTranslate -= 0.25;
}

static void CALLBACK Key_s(void)
{
    doSphere = !doSphere;
    if (doSphere) {
        glTexGeniv(GL_S, GL_TEXTURE_GEN_MODE, sphereMap);
        glTexGeniv(GL_T, GL_TEXTURE_GEN_MODE, sphereMap);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    } else {
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
    }
}

static void CALLBACK Key_0(void)
{
    magFilter = nr;
}

static void CALLBACK Key_1(void)
{
    magFilter = ln;
}

static void CALLBACK Key_2(void)
{
    minFilter = nr;
}

static void CALLBACK Key_3(void)
{
    minFilter = ln;
}

static void CALLBACK Key_4(void)
{
    minFilter = nr_mipmap_nr;
}

static void CALLBACK Key_5(void)
{
    minFilter = nr_mipmap_ln;
}

static void CALLBACK Key_6(void)
{
    minFilter = ln_mipmap_nr;
}

static void CALLBACK Key_7(void)
{
    minFilter = ln_mipmap_ln;
}

static void CALLBACK Draw(void)
{

    glClear(GL_COLOR_BUFFER_BIT);

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrapMode);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapMode);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

    glPushMatrix();

    glTranslatef(0.0, 0.0, zTranslate);
    glRotatef(xRotation, 1, 0, 0);
    glRotatef(yRotation, 0, 1, 0);
    glCallList(cube);

    glPopMatrix();

    glFlush();

    if (doubleBuffer) {
    auxSwapBuffers();
    }
}

static GLenum Args(int argc, char **argv)
{
    GLint i;

    doubleBuffer = GL_FALSE;


    for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-sb") == 0) {
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

    if (texFileName == 0) {
    //printf("No image file.\n");
    auxQuit();
    }

    image = auxRGBImageLoad(texFileName);

    auxInitPosition(0, 0, 300, 300);

    type = AUX_RGB;
    type |= (doubleBuffer) ? AUX_DOUBLE : AUX_SINGLE;

    auxInitDisplayMode(type);

    if (auxInitWindow("Texture Test") == GL_FALSE) {
    auxQuit();
    }

    Init();

    auxExposeFunc((AUXEXPOSEPROC)Reshape);
    auxReshapeFunc((AUXRESHAPEPROC)Reshape);
    auxKeyFunc(AUX_LEFT, Key_LEFT);
    auxKeyFunc(AUX_RIGHT, Key_RIGHT);
    auxKeyFunc(AUX_UP, Key_UP);
    auxKeyFunc(AUX_DOWN, Key_DOWN);
    auxKeyFunc(AUX_T, Key_T);
    auxKeyFunc(AUX_t, Key_t);
    auxKeyFunc(AUX_s, Key_s);
    auxKeyFunc(AUX_0, Key_0);
    auxKeyFunc(AUX_1, Key_1);
    auxKeyFunc(AUX_2, Key_2);
    auxKeyFunc(AUX_3, Key_3);
    auxKeyFunc(AUX_4, Key_4);
    auxKeyFunc(AUX_5, Key_5);
    auxKeyFunc(AUX_6, Key_6);
    auxKeyFunc(AUX_7, Key_7);
    auxMainLoop(Draw);
}
