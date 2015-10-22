//
//  SFPSDOuterGlowEffectLayerInformation.m
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

///#import "SFPSDOuterGlowEffectLayerInformation.h"
#include "SFPSDOuterGlowEffectLayerInformation.h"

SFPSDOuterGlowEffectLayerInformation::SFPSDOuterGlowEffectLayerInformation()
{
    size = 0;
    
    color = EFloat4(0,0,0,0);
    
    blendMode = SFPSDLayerBlendModeNormal;
    enabled = false;
    opacity = 100;
}
void SFPSDOuterGlowEffectLayerInformation::setOpacity(long o)
{
    if (o < 0) {
        o = 0;
    }
    else if (o > 100) {
        o = 100;
    }
    opacity = o;
}
long SFPSDOuterGlowEffectLayerInformation::opacity255()
{
    return (long)(2.55 * (double)opacity);
}
