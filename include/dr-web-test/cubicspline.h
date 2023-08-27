#ifndef CUBICSPLINE_H
#define CUBICSPLINE_H

#include "interpolator.h"
#include "alglib/interpolation.h"

class CubicSpline  : public IInterpolator
{
public:
    CubicSpline();

    virtual float at(float x) override;

    virtual float at(float x, Points& points) override {return at(x);};

    void setData(Points& points);
private:
    alglib::spline1dinterpolant interpolant;


};

#endif // CUBICSPLINE_H
