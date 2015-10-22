//
//  SFPSDSolidFillEffectLayerInformation.h
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

#include "SFPSDLayerBlendModes.h"

#ifndef SFE_H
#define SFE_H

#include "float_base.h"
class SFPSDSolidFillEffectLayerInformation
{
public:
    const char *blendMode;
    EFloat4 color;
private:
    long opacity;
public:
    bool enabled;
public:
    SFPSDSolidFillEffectLayerInformation();
    void setOpacity(long o);
    long opacity255();
};
#endif