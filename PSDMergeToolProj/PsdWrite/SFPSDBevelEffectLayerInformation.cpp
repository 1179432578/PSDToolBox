//
//  SFPSDBevelEffectLayerInformation.m
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

#include "SFPSDBevelEffectLayerInformation.h"

SFPSDBevelEffectLayerInformation::SFPSDBevelEffectLayerInformation()
{
    angle = 0;
    size = 0;
    highlightBlendMode = SFPSDLayerBlendModeNormal;
    shadowBlendMode = SFPSDLayerBlendModeNormal;
    
    highlightColor = EFloat4(0,0,0,0);
    shadowColor = EFloat4(0,0,0,0);
    
    style = SFPSDBevelEffectLayerInformationStyleOuterBevel;
    highlightOpacity = 100;
    shadowOpacity = 100;
    enabled = false;
    useGlobalLight = true;
    direction = SFPSDBevelEffectLayerInformationDirectionDown;
}
void SFPSDBevelEffectLayerInformation::setSize(long s)
{
    if (s < 0) {
        s = 0;
    }
    else if (s > 250) {
        s = 250;
    }
    size = s;
}
void SFPSDBevelEffectLayerInformation::setAngle(long a)
{
    if (a < -360) {
        a = -360;
    }
    else if (a > 360) {
        a = 360;
    }
    angle = a;
}
void SFPSDBevelEffectLayerInformation::setHighlightOpacity(long o)
{
    if (o < 0) {
        o = 0;
    }
    else if (o > 255) {
        o = 255;
    }
    highlightOpacity = o;
}
void SFPSDBevelEffectLayerInformation::setshadowOpacity(long o)
{
    if (o < 0) {
        o = 0;
    }
    else if (o > 255) {
        o = 255;
    }
    shadowOpacity = o;
}
long SFPSDBevelEffectLayerInformation::highlightOpacity255()
{
    return (long)(2.55 * (double)highlightOpacity);
}
long SFPSDBevelEffectLayerInformation::shadowOpacity255()
{
    return (long)(2.55 * (double)shadowOpacity);
}