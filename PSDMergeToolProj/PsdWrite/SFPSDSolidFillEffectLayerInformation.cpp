//
//  SFPSDSolidFillEffectLayerInformation.m
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

///#import "SFPSDSolidFillEffectLayerInformation.h"
#include "SFPSDSolidFillEffectLayerInformation.h"

SFPSDSolidFillEffectLayerInformation::SFPSDSolidFillEffectLayerInformation()
{
    blendMode = SFPSDLayerBlendModeNormal;
    
    color = EFloat4(0,0,0,0);
    
    opacity = 100;
    enabled = false;
}
void SFPSDSolidFillEffectLayerInformation::setOpacity(long o)
{
    if (o < 0) {
        o = 0;
    }
    else if (o > 100) {
        o = 100;
    }
    opacity = o;
}
long SFPSDSolidFillEffectLayerInformation::opacity255()
{
    return (long)(2.55 * (double)opacity);
}
