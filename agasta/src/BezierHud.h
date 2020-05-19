#ifndef __BEZIERHUD_H__
#define __BEZIERHUD_H__

#include "Button.h"
#include "Frames.h"
#include "gl_canvas2d.h"

// Essa classe guarda tudo que envolve parâmetros da curva de Bézier. É ba-
// sicamente um button manager. Responsável por estabelecer a comunicação
// dos dados da curva com as Utils e a curva em si.
class BezierHud{
    private:
        int hudX, hudY, offset;

        // Booleanos e flags dos nossos botões.
        bool activeCP, activeCH, activeG, activeI, activeA;
        bool animatingFlag, clearFlag;

        // Incremento atual armazenado por essa classe.
        float t_inc;

        Button *bPoints, *bHull, *bGraph, *bInterp, *bClear;
        Button *bAnimate, *bPause, *bSlower, *bFaster;
        Frames *fps;

    public:
        BezierHud()
        {
            hudX = 200;
            hudY = 720;
            offset = 20;

            t_inc = 0.5;

            PutInitialState();
            CreateButtons();

            fps = new Frames();
        }

        // Seta o estado inicial das nossas flags.
        void PutInitialState()
        {
            activeCP = activeCH = activeG = true;
            animatingFlag = clearFlag = activeA = activeI = false;
        }

        void CreateButtons()
        {
            bClear = new Button(offset, hudX + offset, 160, 40, 1, 0, 0, (char*)"Clear CPs");

            bGraph = new Button(offset, hudX + 4 * offset, 160, 40, 1, 1, 1, (char*)"Control Graph");
            bPoints = new Button(offset, hudX + 7 * offset, 160, 40, 1, 1, 1, (char*)"Control Points");
            bHull = new Button(offset, hudX + 10 * offset, 160, 40, 1, 1, 1, (char*)"Convex Hull");

            bAnimate = new Button(offset, hudX + 15 * offset, 160, 40, 1, 1, 1, (char*)"Animate");
            bInterp = new Button(offset, hudX + 18 * offset, 160, 40, 1, 1, 1, (char*)"Line Interp.");
            bSlower = new Button(offset, hudX + 21 * offset, 40, 40, 1, 1, 1, (char*)"-");
            bPause = new Button(offset + 60, hudX + 21 * offset, 40, 40, 1, 1, 1, (char*)"|>");
            bFaster = new Button(offset + 120, hudX + 21 * offset, 40, 40, 1, 1, 1, (char*)"+");
        }

        void RenderButtons()
        {
            bPoints->Render(activeCP);
            bHull->Render(activeCH);
            bGraph->Render(activeG);
            bInterp->Render(activeI);
            bAnimate->Render(activeA);

            bSlower->Render(activeA);
            bPause->Render(activeA);
            bFaster->Render(activeA);

            bClear->Render(true);
        }

        void Render()
        {
            CV::color(0.4, 0.4, 0.4);
            CV::rectFill(0, 0, hudX, hudY);

            RenderButtons();
            DrawGraph();
            FPSTextBox();
        }

        // Aplica as mudanças caso o mouse tenha colidido com algum botão.
        bool CheckButtonCollision(int x, int y)
        {
            if     (bPoints->Collided(x, y))  activeCP  = !activeCP;
            else if(bHull->Collided(x, y))    activeCH  = !activeCH;
            else if(bGraph->Collided(x, y))   activeG   = !activeG;
            else if(bAnimate->Collided(x, y)) activeA   = !activeA;
            else if(bClear->Collided(x, y))   clearFlag = true;

            if(activeA) {
                if     (bInterp->Collided(x, y))  activeI   = !activeI;
                else if(bPause->Collided(x, y))   animatingFlag = !animatingFlag;
                else if(bSlower->Collided(x, y) && t_inc > 0.2) t_inc = t_inc - 0.1;
                else if(bFaster->Collided(x, y) && t_inc < 1.0) t_inc = t_inc + 0.1;
            } else activeI = animatingFlag = activeA;
        }

        // Verifica se o mouse se encontra dentro da HUD.
        bool CheckHudCollision(int x, int y)
        {
            return (x <= hudX);
        }

        void DrawGraph()
        {
            CV::color(0.75, 0.75, 0.75);
            CV::rectFill(0, 0, hudX, hudX);

            CV::color(0, 0, 0);
            CV::line(offset - 5, offset, hudX - offset + 5, offset);
            CV::line(hudX - offset, offset + 5, hudX - offset + 5, offset);
            CV::line(hudX - offset, offset - 5, hudX - offset + 5, offset);

            CV::line(offset, offset - 5, offset, hudX - offset + 5);
            CV::line(offset + 5, hudX - offset, offset, hudX - offset + 5);
            CV::line(offset - 5, hudX - offset, offset, hudX - offset + 5);

        }

        // Printa o FPS na tela.
        void FPSTextBox()
        {
            char display[32];
            sprintf(display, "FPS: %.1f", fps->getFrames());

            CV::color(0.75, 0.75, 0.75);
            CV::rectFill(0, hudY - 40, hudX, hudY);

            CV::color(0, 0, 0);
            CV::text(hudX/2 - 5 * strlen(display),
                     hudY - 20 - 5, display);
        }

        bool GetActiveA()       { return activeA; }
        bool GetActiveCP()      { return activeCP; }
        bool GetActiveCH()      { return activeCH; }
        bool GetActiveI()       { return activeI; }
        bool GetActiveG()       { return activeG; }
        bool GetAnimatingFlag() { return animatingFlag; }
        bool GetClearFlag()     { return clearFlag; }

        float GetFPS()          { return fps->getFrames(); }
        float GetTInc()         { return t_inc; }
        int GetGraphX()         { return offset; }
        int GetGraphY()         { return offset; }
        int GetGraphLength()    { return hudX - 2 * offset; }

        void SetAnimatingFlag(bool animatingFlag)
            { this-> animatingFlag = animatingFlag; }

        void ResetClearFlag() { this->clearFlag = false; }


};

#endif // __BEZIERHUD_H__
