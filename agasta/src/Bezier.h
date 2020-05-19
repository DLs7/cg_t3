#ifndef __BEZIER_H__
#define __BEZIER_H__

#include <vector>

#include "BezierUtils.h"
#include "gl_canvas2d.h"

// Essa classe existe *somente* para renderizar/animar a curva de B�zier.
// � a classe "pai" da classe BezierUtils e BezierHud.
class Bezier
{
    private:

    public:
        // Possui uma classe de fun��es utilit�rias e uma classe de HUD,
        // que retorna as informa��es selecionadas e os par�metros para
        // essa classe.
        BezierUtils *bUtils;
        BezierHud *bHud;

        // O t da equea��o de B�zier caso a gente resolva animar a curva.
        float t_incremented;

        Bezier()
        {
            bUtils = new BezierUtils();
            bHud = new BezierHud();
            t_incremented = 0;
        }

        // Fun��o geral que calcula a curva de B�zier.
        void ComputeBezier(std::vector<ControlPoint*> controlPoints, double t_incremented)
        {
            double pastX = 0, pastY = 0;
            // t += 0.001 pareceu um bom misto de performance e precis�o para mim.
            for(float t = 0.0; t <= t_incremented; t += 0.001) {
                double x = 0, y = 0;

                double n = controlPoints.size() - 1;
                for(int i = 0; i < controlPoints.size(); i++) {
                    x += bUtils->Bernstein(n, i) * pow(1 - t, n - i)
                              * pow(t, i) * controlPoints[i]->GetX();
                    y += bUtils->Bernstein(n, i) * pow(1 - t, n - i)
                              * pow(t, i) * controlPoints[i]->GetY();

                    //bUtils->PlotGraph(t, i, bHud->GetGraphX(), bHud->GetGraphY(),
                                      //bHud->GetGraphLength());
                }

                CV::color(1, 0, 0);
                if(t != 0)
                    CV::line(pastX, pastY, x, y);

                pastX = x;
                pastY = y;
            }
        }

        // Renderiza a interpola��o linear que resulta na curva de B�zier caso estejamos
        // animando a mesma.
        void RenderInterpolation()
        {
            if(bHud->GetActiveI()) {
                // Eu converto meus ControlPoints para Points. Eu explico isso melhor nas
                // headers dessas duas classes.
                std::vector<Point*> points;
                for(auto cp : bUtils->GetPoints()) {
                    points.push_back(new Point(cp->GetX(), cp->GetY()));
                }
                bUtils->LineInterpolation(points, t_incremented);
            }
        }

        // Bezier sem anima��o
        void RenderStaticBezier()
        {
            ComputeBezier(bUtils->GetPoints(), 1);
            bUtils->PlotGraph(1, bHud->GetGraphX(), bHud->GetGraphY(), bHud->GetGraphLength());
        }

        // Essa fun��o anima a curva de B�zier. � baseada basicamente na demo 3. Nada muito
        // especial.
        void AnimateBezier()
        {
            if (t_incremented < 1.0){
                t_incremented += (bHud->GetTInc() * 1/bHud->GetFPS()) * bHud->GetAnimatingFlag();

                RenderInterpolation();
                ComputeBezier(bUtils->GetPoints(), t_incremented);
                bUtils->PlotGraph(t_incremented,
                                  bHud->GetGraphX(), bHud->GetGraphY(), bHud->GetGraphLength());
            } if (t_incremented >= 1.0){
                // Esse trecho � meio XGH, mas � o que funcionou.
                t_incremented = 1.0;

                RenderInterpolation();
                RenderStaticBezier();

                if(!bHud->GetAnimatingFlag()) {
                    t_incremented = 0.0;
                    bHud->SetAnimatingFlag(true);
                }
            }
        }

        void RenderBezier()
        {
            if (bUtils->GetPoints().size() >= 2){
                if(bHud->GetActiveA()) AnimateBezier();
                else {
                    RenderStaticBezier();
                    t_incremented = 0;
                }
            }
        }

        // Renderiza bUtils caso seus respectivos par�metros sejam true na bHud.
        void RenderUtils()
        {
            if(bHud->GetActiveG())  bUtils->ConnectControlPoints();
            if(bHud->GetActiveCH()) bUtils->DrawConvexHull();

            if(bHud->GetClearFlag()) {
                bUtils->ClearControlPoints();
                bHud->ResetClearFlag();
            }
        }

        void RenderControlPoints()
        {
            if(bHud->GetActiveCP())
                for(auto p : bUtils->GetPoints()) p->Render();
        }

        void Render()
        {
            bHud->Render();

            RenderUtils();
            RenderBezier();
            RenderControlPoints();
        }
};

#endif
