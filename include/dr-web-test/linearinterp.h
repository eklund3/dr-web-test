#ifndef LINEARINTERP_H
#define LINEARINTERP_H

#include "interpolator.h"

class LinearInterp : public IInterpolator
{
public:
    LinearInterp();

    virtual float at(float x, Points& points) override;

private:
    virtual float at(float x) override {return 0;};

};

#endif // LINEARINTERP_H
