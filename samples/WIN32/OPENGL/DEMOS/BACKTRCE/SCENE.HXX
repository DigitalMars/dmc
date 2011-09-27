/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 *               1993-1995 Microsoft Corporation
 *
 * ALL RIGHTS RESERVED
 *
 * Please refer to OpenGL/readme.txt for additional information
 *
 */
#ifndef SCENE_H
#define SCENE_H

#include "point.hxx"

/* Index of refraction stuff */
const GLfloat min_index = .1;
const GLfloat max_index = 2.5;
const int nindices = 6;
const struct {
  const char *name;
  GLfloat index;
} indices[] = {
  {"Air", 1.0},
  {"Ice", 1.31},
  {"Water", 1.33},
  {"Zinc Crown Glass", 1.517}, 
  {"Light Flint Glass", 1.650},
  {"Heavy Flint Glass", 1.890}
};
const int def_refraction_index = 4;

typedef struct {
  GLfloat diffuse[4];
  Point pos;
  GLboolean shadow_mask[4];
  GLfloat matrix[16];
  char name[64];
  int on;
} light;
const int nlights = 3;
//extern light lights[];

/* scene_load_texture uses no OpenGL calls and so can be called before
 * mapping the window */
int scene_load_texture(char *texfile);
//const char def_texfile[] = DATADIR "floor.rgb";
const char def_texfile[] = "floor.rgb";

extern int possible_divisions[];
const int npossible_divisions = 4;
const int def_divisions_index = 1;

extern GLfloat aspect;

extern int draw_square, draw_shadows, draw_refraction, draw_sphere, 
  draw_lights, draw_texture;

/* These are names used for picking */
const int name_background = 0;
const int name_square = 1;
const int name_sphere = 2;
const int name_lights = 3;

void scene_init();
void scene_draw();
int scene_pick(GLdouble x, GLdouble y);

/* This returns all the lights to their default postions */
void scene_reset_lights();

void lights_onoff(int light, int val);
void refraction_change(GLfloat refraction);
void divisions_change(int divisions);

/* name is one of the names defined above.
 * phi, theta are in radians and are the amount of motion requested 
 * returns whether or not it did anything. 
 * If update is zero, the shadows and refractions will not be recomputed */
int scene_move(int name, float dr, float dphi, float dtheta, int update);

/* This function is a companion to scene_move().
 * It recomputes the shadows and refractions - dr, dphi, and dtheta
 * should indicate whether or not the respective values were changed
 * since the stuff was last computed. */
void scene_move_update(int name, int dr, int dphi, int dtheta);

#endif
