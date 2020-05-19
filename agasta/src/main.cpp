/***************************************************************************************
// TRABALHO C - AUGUSTO GAI DAL'ASTA (MATR. 201810291)
//
// Este trabalho conta com as seguintes funcionalidades essenciais:
// > Inserção de pontos de controle;
// > Edição da posição de qualquer ponto de controle;
// > Visualização das blending functions;
// > Visualização do grafo de controle;
// > Visualização estática da curva gerada;
// > Visualização animada da curva gerada.
//
// Este trabalho também conta com as seguintes funcionalidades adicionais:
// > Possibilidade de pausar/continuar a animação da curva.
// > Possibilidade de aumentar ou diminuir a velocidade que a curva é gerada;
// > Edição dos pontos de controle em atualização na curva tempo real;
// > Visualização do fecho convexo.
// > Visualização da interpolação das linhas que resultam na curva de Bézier.
//
// INSTRUÇÕES:
// > Clique com o botão esquerdo do mouse em qualquer parte da cena -> Cria um ponto de
//   controle;
// > Segurar o botão esquerdo do mouse sobre um ponto de controle -> Arrasta este ponto
//   de controle;
// > Botão direito do mouse sobre um ponto de controle -> Remove este ponto de controle;
// > Botão "Clear CPs" -> Limpa todos os pontos de controle;
// > Botão "Control Graph" -> Mostra/esconde o grafo de controle dos pontos de controle;
// > Botão "Control Points" -> Mostra/esconde os pontos de controle;
// > Botão "Convex Hull" -> Mostra/esconde o fecho convexo;
// > Botão "Animate" -> Anima a curva se ativado, e a mostra estaticamente se desativado.
// Este botão também ativa os botões "-", "|>" (play), "+" e "Line interpolation";
//   >> Botão "Line Interpolation" -> Mostra a interpolação das linhas que resultam na
//   curva de Bezier gerada durante a animação;
//   >> Botão "-" -> Diminui a velocidade de animação da curva;
//   >> Botão "+" -> Aumenta a velocidade de animação da curva;
//   >> Botão "|>" -> Pausa/continua a animação da curva.
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

// Variável que cria um objeto de classe Bézier. Essa classe é responsável somente por
// gerar a nossa curva e animar ela. Todas as funções extras são tratadas em duas sub-
// classes: BezierHud e BezierUtils.
Bezier *b = NULL;

// Variável que define se estamos arrastando um ponto de controle ou não.
bool controlPointDrag = false;

// Render
void render()
{
   CV::clear(0, 0, 0);
   b->Render();
}

// Função chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
}

// Função chamada toda vez que uma tecla for liberada.
void keyboardUp(int key)
{
}

// Função para tratamento de mouse: cliques, movimentos e arrastos.
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    if(button == 0) {
        if(state == 0)
        {
            // Checa se o clique colidiu com a HUD,
            if(!b->bHud->CheckHudCollision(x, y)) {
                // Se não, checa se colidiu com algum ponto,
                if(!b->bUtils->CheckPointCollision(x, y))
                    // Se não, adiciona um novo ponto de controle.
                    b->bUtils->AddControlPoint(x, y);
                // Se sim, habilita o arrasto do ponto de controle selecionado.
                else controlPointDrag = true;
            // Se sim, verifica se ele colidiu com os botões.
            } else {
                b->bHud->CheckButtonCollision(x, y);
            }
        // Se soltou botão do mouse, desabilitou o arrasto.
        } else if(state == 1) controlPointDrag = false;
    // Se houve clique com o botão direito, verifica se colidiu com um dos pon-
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
