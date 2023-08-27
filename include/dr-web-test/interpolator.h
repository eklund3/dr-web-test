#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include "datastructs.h"

class IInterpolator
{
protected:
    virtual ~IInterpolator() = default;

public:
    virtual float at(float x, Points& points) = 0;
    virtual float at(float x) = 0;

    IInterpolator& operator=(const IInterpolator&) = delete;
};

#endif // INTERPOLATOR_H
