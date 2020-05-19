#ifndef __BEZIERUTILS_H__
#define __BEZIERUTILS_H__

#include <vector>

#include "BezierHud.h"
#include "ControlPoint.h"
#include "Frames.h"
#include "Point.h"
#include "gl_canvas2d.h"

// Essa classe guarda todas as equações chatas que não envolvem diretamente
// a curva de Bézier. Aqui a gente lida com os pontos de controle, o fecho
// convexo, etc...
class BezierUtils{
    private:
        // Armazenamos todos os pontos de controle nessa classe.
        std::vector<ControlPoint*> controlPoints, convexHull;

    public:
        BezierUtils()
        {
        }

        /******************************************************************
        *********************** CONTROL POINT UTILS ***********************
        *******************************************************************/

        void AddControlPoint(int x, int y)
        {
            UnselectAllControlPoints();
            controlPoints.push_back(new ControlPoint(x, y, controlPoints.size()));

            SetConvexHull();
        }

        void DeleteControlPoint(int x, int y)
        {
            int i = 0;
            for(int i = 0; i < controlPoints.size(); i++) {
                if(controlPoints[i]->Collided(x, y)) {
                   delete controlPoints[i];
                   controlPoints.erase(controlPoints.begin() + i);
                }
            }
            RebuildLabels();
            SetConvexHull();
        }

        bool CheckPointCollision(int x, int y)
        {
            for(auto p : controlPoints) {
                if(p->Collided(x, y)) {
                    UnselectAllControlPoints();
                    p->SetSelected(true);

                    return true;
                }
            }
            return false;
        }

        void ClearControlPoints()
        {
            for (int i = 0; i < controlPoints.size(); i++){
                delete controlPoints[i];
            }
            controlPoints.clear();

            for (int i = 0; i < convexHull.size(); i++){
                delete convexHull[i];
            }
            convexHull.clear();
        }

        // Grafo de controle.
        void ConnectControlPoints()
        {
            CV::color(0.75, 0.75, 0.75);
            for(int i = 1; i < controlPoints.size(); i++) {
                CV::line(controlPoints[i-1]->GetX(), controlPoints[i-1]->GetY(),
                         controlPoints[i]->GetX()  , controlPoints[i]->GetY());
            }
        }

        void DragControlPoint(int x, int y)
        {
            for(auto p : controlPoints) {
                if(p->GetSelected()) {
                    p->SetX(x);
                    p->SetY(y);
                }
            }
            SetConvexHull();
        }

        // Arruma a ordem numérica do display dos pontos caso algum seja
        // deletado.
        void RebuildLabels()
        {
            int i = 0;
            for(auto p : controlPoints) {
                p->SetLabel(i);
                i++;
            }
        }

        void UnselectAllControlPoints()
        {
            for(auto p : controlPoints) p->SetSelected(false);
        }

        std::vector<ControlPoint*> GetPoints() { return controlPoints; }

        /******************************************************************
        ************************* CONVEX HULL UTILS ***********************
        *******************************************************************/

        int GetOrientation(ControlPoint* p1, ControlPoint* p2, ControlPoint* p3)
        {
            int val = (p2->GetY() - p1->GetY()) * (p3->GetX() - p2->GetX()) -
                      (p2->GetX() - p1->GetX()) * (p3->GetY() - p2->GetY());

            if (val == 0) return 0;
            return (val > 0) ? 1: 2;
        }

        // Cria o fecho convexo.
        void SetConvexHull()
        {
            convexHull.clear();
            if (controlPoints.size() < 3) return;

            int l = 0;
            for (int i = 1; i < controlPoints.size(); i++)
                if (controlPoints[i]->GetX() < controlPoints[l]->GetX())
                    l = i;

            int p = l, q;
            do
            {
                convexHull.push_back(controlPoints[p]);

                q = (p + 1) % controlPoints.size();
                for (int i = 0; i < controlPoints.size(); i++)
                {
                   if (GetOrientation(controlPoints[p], controlPoints[i], controlPoints[q]) == 2)
                       q = i;
                }
                p = q;

            } while (p != l);
        }

        void DrawConvexHull()
        {
            if(convexHull.size() < 3) return;

            CV::color(0, 1, 0);

            int i;
            for(i = 1; i < convexHull.size(); i++) {
                CV::line(convexHull[i - 1]->GetX(), convexHull[i - 1]->GetY(),
                         convexHull[i]->GetX(),     convexHull[i]->GetY());
            }

            CV::line(convexHull[i - 1]->GetX(), convexHull[i - 1]->GetY(),
                     convexHull[0]->GetX(),     convexHull[0]->GetY());
        }

        /******************************************************************
        ******************************* GRAPH *****************************
        *******************************************************************/

        double Factorial(int n)
        {
            if (n == 0) return 1;
            else return n * Factorial(n - 1);
        }

        double Bernstein(float n, float i)
        {
            return (Factorial(n)/(Factorial(i) * Factorial(n - i)));
        }

        // Essa função plota o gráfico das Blending Functions. Não tem nada
        // de muito complexo nela. Seu funcionamento é similar ao da própria
        // curva Bezier.
        void PlotGraph(float t_incremented, int x, int y, int length)
        {
            std::vector<double> pastP;
            pastP.push_back(1); // De novo, provavelmente XGH, mas funcionou.

            // t += 0.005 pareceu um bom misto de performance e precisão para mim.
            for(float t = 0.0; t <= t_incremented; t += 0.005) {
                double n = controlPoints.size() - 1;
                for(int i = 0; i < controlPoints.size(); i++) {
                    double point = Bernstein(n, i);

                    point *= pow(t, i) * pow((1 - t), (n - i));

                    CV::color(0, 0, 0);
                    CV::line(x + length * (t - 0.01), y + length * pastP[i],
                             x + length * t, x + length * point);

                    pastP.push_back(point);
                }
                pastP.clear();
            }
        }

        /******************************************************************
        *************************** INTERPOLATION *************************
        *******************************************************************/

        // Faz a interpolação de linhas na animação.
        void LineInterpolation(std::vector<Point*> points, double t){
            if (points.size() <= 1) return;

            std::vector<Point*> interpolatedPoints;

            for(int i = 0; i < points.size() - 1; i++){
                Point *p1 = points[i];
                Point *p2 = points[i+1];

                double x, y;
                x = (1 - t) * p1->GetX() + t * p2->GetX();
                y = (1 - t) * p1->GetY() + t * p2->GetY();

                Point *p = new Point(x, y, p1->GetX(), p1->GetY(),
                                           p2->GetX(), p2->GetY());
                interpolatedPoints.push_back(p);

                p->Render();
            }

            LineInterpolation(interpolatedPoints, t);
            for (int i = 0; i < interpolatedPoints.size(); i++){
                delete interpolatedPoints[i];
            }
            interpolatedPoints.clear();
        }

};

#endif
