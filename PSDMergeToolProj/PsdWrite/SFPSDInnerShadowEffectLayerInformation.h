//
//  SFPSDInnerShadowEffectLayerInformation.h
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

#include "SFPSDLayerBlendModes.h"

#ifndef ISE_H
#define ISE_H

#include "float_base.h"
class SFPSDInnerShadowEffectLayerInformation
{
public:
    EFloat4 color;
    const char *blendMode;
    bool enabled;
    bool useGlobalLight;
private:
    long size;
    long angle;
    long distance;
    long opacity;
public:
    SFPSDInnerShadowEffectLayerInformation();
    void setSize(long s);
    void setAngle(long a);
    void setDistance(long d);
    void setOpacity(long o);
    long opacity255();
    inline long getSize() const { return size; }
    inline long getAngle() const { return angle; }
    inline long getDistance() const { return distance; }
};

#endif