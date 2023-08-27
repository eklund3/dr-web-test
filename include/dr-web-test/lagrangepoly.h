#ifndef LAGRANGEPOLY_H
#define LAGRANGEPOLY_H

#include "interpolator.h"

class LagrangePoly : public IInterpolator
{
public:
    LagrangePoly();

    virtual float at(float x, Points& points) override;

private:
    virtual float at(float x) override {return 0;};
};

#endif // LAGRANGEPOLY_H
