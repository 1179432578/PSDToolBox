#ifndef FLOAT_BASE_H
#define FLOAT_BASE_H

#include "float_base.h"

#define FLOAT_TOLERANCE 0.001f

float float_abs(float f)
{
    if (f < 0.0f)
        return -f;
    else
        return f;
}

bool float_equal_float(float ft0, float ft1)
{
    float t = ft0 - ft1;
    if (float_abs(t) < FLOAT_TOLERANCE)
        return true;
    else
        return false;
}

#endif