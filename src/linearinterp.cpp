#include "dr-web-test/linearinterp.h"
#include "qdebug.h"

LinearInterp::LinearInterp()
{

}

float LinearInterp::at(float x, Points& points)
{
    //ищем две ближайшие точки
    float fx = 0.0;
    for (int i = 0; i < points.size(); i++) {
        if (points[i].x > x) {
            fx = ((x-points[i-1].x)*(points[i].y-points[i-1].y) / (points[i].x - points[i-1].x)) + points[i-1].y;
            break;
        }
    }
    return fx;
}
