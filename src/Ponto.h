#ifndef __PONTO_H__
#define __PONTO_H__

#include "gl_canvas2d.h"
#include <vector>

class Ponto{
public:
   std::vector<Ponto*> pontos;
   float x, y;
   Ponto()
   {
      x = y = 0;
   }
   Ponto(float _x, float _y)
   {
      x = _x;
      y = _y;
   }
   void set(float _x, float _y)
   {
       x = _x;
       y = _y;
   }

    float distancia(Ponto p) {
        return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
    }
    Ponto operator - (const Ponto& v)
   {
        Ponto aux( x - v.x, y - v.y);
        return( aux );
   }

};

#endif
