//
//  SFPSDDropShadowEffectLayerInformation.m
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

#include "SFPSDDropShadowEffectLayerInformation.h"

SFPSDDropShadowEffectLayerInformation::SFPSDDropShadowEffectLayerInformation()
{
    size = 0;
    angle = 0;
    distance = 0;
    
    color = EFloat4(0,0,0,0);
    
    blendMode = SFPSDLayerBlendModeNormal;
    enabled = false;
    useGlobalLight = false;
    opacity = 100;
}
void SFPSDDropShadowEffectLayerInformation::setOpacity(long o)
{
    if (o < 0) {
        o = 0;
    }
    else if (o > 100) {
        o = 100;
    }
    opacity = o;
}
long SFPSDDropShadowEffectLayerInformation::opacity255()
{
    return (long)(2.55 * (double)opacity);
}
void SFPSDDropShadowEffectLayerInformation::setSize(long s)
{
    if (s < 0) {
        s = 0;
    }
    else if (s > 250) {
        s = 250;
    }
    size = s;
}
void SFPSDDropShadowEffectLayerInformation::setAngle(long a)
{
    if (a < -360) {
        a = -360;
    }
    else if (a > 360) {
        a = 360;
    }
    angle = a;
}
void SFPSDDropShadowEffectLayerInformation::setDistance(long d)
{
    if (d < 0) {
        d = 0;
    }
    else if (d > 30000) {
        d = 30000;
    }
    distance = d;
}