#include "dr-web-test/lagrangepoly.h"

#include <QDebug>

LagrangePoly::LagrangePoly()
{

}

float LagrangePoly::at(float x, Points& points)
{
    double result = 0.0;
    size_t n = points.size();
    for (int i = 0; i < n; i++) {
       double term = points[i].y;
       for (int j = 0; j < n; j++) {
          if (j != i) {
             double diff = points[i].x - points[j].x;
             term = term * (x - points[j].x) / double(points[i].x - points[j].x);
          }
       }
       result += term;
    }
    return result;

}

