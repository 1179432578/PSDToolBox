//
//  SFPSDDropShadowEffectLayerInformation.h
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

#include "SFPSDLayerBlendModes.h"

#ifndef DSE_H
#define DSE_H

#include "float_base.h"
class SFPSDDropShadowEffectLayerInformation
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
    SFPSDDropShadowEffectLayerInformation();
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
