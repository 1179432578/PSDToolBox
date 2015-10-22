//
//  SFPSDBevelEffectLayerInformation.h
//  SFPSDWriter Mac OS X
//
//  Created by Konstantin Erokhin on 15/06/14.
//  Copyright (c) 2014 Shiny Frog. All rights reserved.
//

#include "SFPSDLayerBlendModes.h"

#ifndef BE_H
#define BE_H

typedef enum {
    SFPSDBevelEffectLayerInformationStyleOuterBevel = 1,
    SFPSDBevelEffectLayerInformationStyleInnerBevel = 2,
    SFPSDBevelEffectLayerInformationStyleEmboss = 3,
    SFPSDBevelEffectLayerInformationStylePillowEmboss = 4,
    SFPSDBevelEffectLayerInformationStyleStrokeEmboss = 5
} SFPSDBevelEffectLayerInformationStyle;

typedef enum {
    SFPSDBevelEffectLayerInformationDirectionUp = 0,
    SFPSDBevelEffectLayerInformationDirectionDown = 1
    
} SFPSDBevelEffectLayerInformationDirection;

#include "float_base.h"
class SFPSDBevelEffectLayerInformation
{
public:
    EFloat4 highlightColor;
    EFloat4 shadowColor;
    const char *highlightBlendMode;
    const char *shadowBlendMode;
    SFPSDBevelEffectLayerInformationStyle style;
    bool enabled;
    bool useGlobalLight;
    SFPSDBevelEffectLayerInformationDirection direction;
private:
    long size;
    long angle;
    long distance;
    long highlightOpacity;
    long shadowOpacity;
public:
    SFPSDBevelEffectLayerInformation();
    void setSize(long s);
    void setAngle(long a);
    void setHighlightOpacity(long o);
    void setshadowOpacity(long o);
    long highlightOpacity255();
    long shadowOpacity255();
    inline long getSize() const { return size; }
    inline long getDistance() const { return distance; }
    inline long getAngle() const { return angle; }
};

#endif
