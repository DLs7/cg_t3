/***************************************************************************************
// TRABALHO C - AUGUSTO GAI DAL'ASTA (MATR. 201810291)
//
// Este trabalho conta com as seguintes funcionalidades essenciais:
// > Inser��o de pontos de controle;
// > Edi��o da posi��o de qualquer ponto de controle;
// > Visualiza��o das blending functions;
// > Visualiza��o do grafo de controle;
// > Visualiza��o est�tica da curva gerada;
// > Visualiza��o animada da curva gerada.
//
// Este trabalho tamb�m conta com as seguintes funcionalidades adicionais:
// > Possibilidade de pausar/continuar a anima��o da curva.
// > Possibilidade de aumentar ou diminuir a velocidade que a curva � gerada;
// > Edi��o dos pontos de controle em atualiza��o na curva tempo real;
// > Visualiza��o do fecho convexo.
// > Visualiza��o da interpola��o das linhas que resultam na curva de B�zier.
//
// INSTRU��ES:
// > Clique com o bot�o esquerdo do mouse em qualquer parte da cena -> Cria um ponto de
//   controle;
// > Segurar o bot�o esquerdo do mouse sobre um ponto de controle -> Arrasta este ponto
//   de controle;
// > Bot�o direito do mouse sobre um ponto de controle -> Remove este ponto de controle;
// > Bot�o "Clear CPs" -> Limpa todos os pontos de controle;
// > Bot�o "Control Graph" -> Mostra/esconde o grafo de controle dos pontos de controle;
// > Bot�o "Control Points" -> Mostra/esconde os pontos de controle;
// > Bot�o "Convex Hull" -> Mostra/esconde o fecho convexo;
// > Bot�o "Animate" -> Anima a curva se ativado, e a mostra estaticamente se desativado.
// Este bot�o tamb�m ativa os bot�es "-", "|>" (play), "+" e "Line interpolation";
//   >> Bot�o "Line Interpolation" -> Mostra a interpola��o das linhas que resultam na
//   curva de Bezier gerada durante a anima��o;
//   >> Bot�o "-" -> Diminui a velocidade de anima��o da curva;
//   >> Bot�o "+" -> Aumenta a velocidade de anima��o da curva;
//   >> Bot�o "|>" -> Pausa/continua a anima��o da curva.
//
****************************************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Bezier.h"
#include "BezierUtils.h"

// Largura e altura iniciais da tela. Alteram com o redimensionamento de tela.
int screenWidth = 1280, screenHeight = 720;

// Vari�vel que cria um objeto de classe B�zier. Essa classe � respons�vel somente por
// gerar a nossa curva e animar ela. Todas as fun��es extras s�o tratadas em duas sub-
// classes: BezierHud e BezierUtils.
Bezier *b = NULL;

// Vari�vel que define se estamos arrastando um ponto de controle ou n�o.
bool controlPointDrag = false;

// Render
void render()
{
   CV::clear(0, 0, 0);
   b->Render();
}

// Fun��o chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
}

// Fun��o chamada toda vez que uma tecla for liberada.
void keyboardUp(int key)
{
}

// Fun��o para tratamento de mouse: cliques, movimentos e arrastos.
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    if(button == 0) {
        if(state == 0)
        {
            // Checa se o clique colidiu com a HUD,
            if(!b->bHud->CheckHudCollision(x, y)) {
                // Se n�o, checa se colidiu com algum ponto,
                if(!b->bUtils->CheckPointCollision(x, y))
                    // Se n�o, adiciona um novo ponto de controle.
                    b->bUtils->AddControlPoint(x, y);
                // Se sim, habilita o arrasto do ponto de controle selecionado.
                else controlPointDrag = true;
            // Se sim, verifica se ele colidiu com os bot�es.
            } else {
                b->bHud->CheckButtonCollision(x, y);
            }
        // Se soltou bot�o do mouse, desabilitou o arrasto.
        } else if(state == 1) controlPointDrag = false;
    // Se houve clique com o bot�o direito, verifica se colidiu com um dos pon-
    // tos existentes e deleta o mesmo.
    } else if(button == 2) {
        b->bUtils->DeleteControlPoint(x, y);
    // Se o arrasto estiver habilitado, arrasta o ponto de controle selecionado.
    } else if(button == -2) {
        if(controlPointDrag && x > 200)
            b->bUtils->DragControlPoint(x, y);
    }
}

// Main
int main(void)
{
    b = new Bezier();

    CV::init(&screenWidth, &screenHeight, "TC - Augusto Gai Dal'Asta");
    CV::run();
}
