//
//  SFPSDOuterGlowEffectLayerInformation.h
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

#include "SFPSDLayerBlendModes.h"

#ifndef OGE_H
#define OGE_H

#include "float_base.h"
class SFPSDOuterGlowEffectLayerInformation
{
public:
    long size;
    EFloat4 color;
    const char *blendMode;
    bool enabled;
private:
    long opacity;
public:
    SFPSDOuterGlowEffectLayerInformation();
    void setOpacity(long o);
    long opacity255();
};

#endif