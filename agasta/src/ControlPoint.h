#ifndef __CONTROLPOINT_H__
#define __CONTROLPOINT_H__

#include <math.h>
#include "gl_canvas2d.h"

#define RADIUS 7.5
#define DIV 25.0

// A classe ControlPoint � uma classe mais extensa que a classe Point.
// Nela encontram-se fun��es pra manipular os pontos. � poss�vel mudar
// a posi��o, criar e deletar eles muito mais facilmente, diferente de
// Points, que s� existem para a interpola��o, sem abertura pra nenhum
// tipo de manipula��o.
class ControlPoint
{
    private:
        double x, y;
        int label;
        char *pn;

        bool selected;

    public:
        ControlPoint(double x, double y, int label)
        {
            this->label = label;
            this->pn = (char*)malloc(sizeof(char) * 5);
            sprintf(pn, "P%d", label);

            this->x = x;
            this->y = y;

            selected = true;
        }

        void Render()
        {
            CV::color(1, 1, 1);
            CV::text(x - 5 * strlen(pn), y + RADIUS * 2, pn);
            CV::circleFill(x, y, RADIUS, DIV);

            if(selected) CV::circle(x, y, 1.5 * RADIUS, DIV);
        }

        // Verifica se tal ponto � o selecionado.
        bool GetSelected() {
            return this->selected;
        }

        // Existe basicamente para que, caso algum ponto seja deletado,
        // seja f�cil rearranjar o label certo para o ControlPoint em
        // um vector.
        void SetLabel(int label)
        {
            this->label = label;
            sprintf(pn, "P%d", label);
        }

        void SetSelected(bool selected)
        {
            this->selected = selected;
        }

        void SetX(double x) { this->x = x; }
        void SetY(double y) { this->y = y; }

        double GetX() { return this->x; }
        double GetY() { return this->y; }

        // Retorna true caso o mouse colida com o ControlPoint.
        bool Collided(int x, int y)
        {
            return abs(sqrt(pow(this->x - x, 2.0) + pow(this->y - y, 2.0))) <= 1.5 * RADIUS;
        }
};

#endif
