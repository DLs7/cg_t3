#ifndef __POINT_H__
#define __POINT_H__

#include <math.h>
#include <stdlib.h>
#include "gl_canvas2d.h"

#define P_RADIUS 5
#define DIV 25.0

// A classe Point é muito mais simples que a classe ControlPoint.
// Ela serve básica e puramente para criar esferas na interpolação
// linear. Simplesmente uma versão básica e sem possibilidade de in-
// teração, diferente de ControlPoints.
class Point
{
    private:
        double x, y;
        double x1, y1, x2, y2;

    public:
        Point(double x, double y)
        {
            this->x = x;
            this->y = y;
        }

        Point(double x, double y,
              double x1, double y1, double x2, double y2)
        {
            this->x = x;
            this->y = y;
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;
        }

        // Renderiza a linha conectando com o ponto anterior.
        void Render()
        {
            CV::color(0.25, 0.25, 0.25);
            CV::circleFill(x, y, P_RADIUS, DIV);
            CV::line(x1, y1, x2, y2);
        }

        void SetX(double x) { this->x = x; }
        void SetY(double y) { this->y = y; }

        double GetX() { return this->x; }
        double GetY() { return this->y; }
};

#endif
