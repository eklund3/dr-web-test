#include "dr-web-test/cubicspline.h"
#include "dr-web-test/alglib/interpolation.h"
#include <algorithm>
#include <functional>
#include <QDebug>

CubicSpline::CubicSpline()
{

}

float CubicSpline::at(float x)
{
    return alglib::spline1dcalc(interpolant, x);
}

void CubicSpline::setData(Points &points)
{
    std::vector<double> arr_x_std;
    std::transform(points.begin(), points.end(), std::back_inserter(arr_x_std),
               std::mem_fn(&Point::x));
    std::vector<double> arr_y_std;
    std::transform(points.begin(), points.end(), std::back_inserter(arr_y_std),
               std::mem_fn(&Point::y));

    alglib::real_1d_array arr_x;
    alglib::real_1d_array arr_y;
    arr_x.setcontent(arr_x_std.size(), &arr_x_std.front());
    arr_y.setcontent(arr_y_std.size(), &arr_y_std.front());

    alglib::spline1dbuildcubic(arr_x, arr_y, interpolant);
}
