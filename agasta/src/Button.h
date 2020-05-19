#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "gl_canvas2d.h"

class Button{
    private:
      // Armazena a altura e a largura do bot�o, assim como booleanos que identificam a cor do mesmo.
      // Tamb�m armazena o seu label.
      int width, height, x, y;
      float r, g, b;
      char label[32];

    public:
        // Cria um bot�o dado uma largura, uma altura, um label e booleanos r, g, b que definem a cor.
        // A posi��o do bot�o n�o � dada, uma vez que a posi��o dele constantemente atualiza.
        // Essa posi��o � passada no m�todo Render.
        Button(int x, int y, int width, int height, float r, float g, float b, char *label)
        {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;

            strcpy(this->label, label);

            this->r = r;
            this->g = g;
            this->b = b;
        }

        // Renderiza o bot�o.
        // O active intensifica a cor dele caso seja true (ou seja, ele est� ativado), e o x0, y0
        // definem a posi��o deles baseada no tamanho da tela.
        void Render(bool active)
        {
            CV::color(r/2 + r/2 * active,
                      g/2 + g/2 * active,
                      b/2 + b/2 * active);
            CV::rectFill(x, y , x + width, y + height);

            CV::color(0, 0, 0);
            CV::text(x + width/2 - 5 * strlen(label), y + height/2 - 5, label); //escreve o label do botao mais ou menos ao centro.
        }

        // Recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse est� em cima do bot�o.
        bool Collided(int mx, int my)
        {
            return(mx >= x && mx <= (x + width) && my >= y && my <= (y + height));
        }
};

#endif
