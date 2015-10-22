//
//  SFPSDInnerGlowEffectLayerInformation.m
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

#include "SFPSDInnerGlowEffectLayerInformation.h"


SFPSDInnerGlowEffectLayerInformation::SFPSDInnerGlowEffectLayerInformation()
{
    size = 0;
    
    color = EFloat4(0,0,0,0);
    
    blendMode = SFPSDLayerBlendModeNormal;
    enabled = false;
    opacity = 100;
    source = SFPSDInnerGlowEffectLayerInformationSourceCenter;
}
void SFPSDInnerGlowEffectLayerInformation::setOpacity(long o)
{
    if (o < 0) {
        o = 0;
    }
    else if (o > 100) {
        o = 100;
    }
    opacity = o;
}
long SFPSDInnerGlowEffectLayerInformation::opacity255()
{
    return (long)(2.55 * (double)opacity);
}
void SFPSDInnerGlowEffectLayerInformation::setSize(long s)
{
    if (s < 0) {
        s = 0;
    }
    else if (s > 250) {
        s = 250;
    }
    size = s;
}
