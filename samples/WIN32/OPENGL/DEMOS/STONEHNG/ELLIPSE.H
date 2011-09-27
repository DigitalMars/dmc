#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Stone.h"

const int nsarcens = 8;
const int nlintels = 4;

class EllipseSt {
 public:
  EllipseSt();
  ~EllipseSt() {};
  
  void erode(float p);
  
  void draw();
  void draw_shadow(Point dlight, GLfloat blur = 0.0,
                   Color color = black, Color diffuse = black);
  
 private:
  GLfloat r1, r2;
  
  Stone sarcens[nsarcens], lintels[nlintels];
  
  void copy_zero();
  
  void draw_sarcens();
  void draw_lintels();
  
  void draw_sarcens_shadows(Point dlight, GLfloat blur,
                            Color color, Color diffuse);
  void draw_lintels_shadows(Point dlight, GLfloat blur,
                            Color color, Color diffuse);
};

#endif
