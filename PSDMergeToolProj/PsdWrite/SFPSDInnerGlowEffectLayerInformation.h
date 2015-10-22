//
//  SFPSDInnerGlowEffectLayerInformation.h
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

#include "SFPSDLayerBlendModes.h"

#ifndef IGE_H
#define IGE_H

typedef enum {
    SFPSDInnerGlowEffectLayerInformationSourceCenter = 0,
    SFPSDInnerGlowEffectLayerInformationSourceEdge = 1
} SFPSDInnerGlowEffectLayerInformationSource;

#include "float_base.h"
class SFPSDInnerGlowEffectLayerInformation
{
public:
    long size;
    EFloat4 color;
    const char *blendMode;
    bool enabled;
    SFPSDInnerGlowEffectLayerInformationSource source;
private:
    long opacity;
public:
    SFPSDInnerGlowEffectLayerInformation();
    void setSize(long s);
    void setOpacity(long o);
    long opacity255();
    inline long getSize() const { return size; }
};

#endif