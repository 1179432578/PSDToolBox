//
//  SFPSDLayer.h
//  SFPSDWriter
//
//  Created by Konstantin Erokhin on 06/06/13.
//  Copyright (c) 2013 Shiny Frog. All rights reserved.
//
//  Inspired by PSDWriter by Ben Gotow ( https://github.com/bengotow/PSDWriter )
//

#include "SFPSDLayerBlendModes.h"
#include "SFPSDEffectsLayerEffectSignatures.h"

#include "SFPSDEffectLayerInformations.h"
#include <vector>
#include <string>

#ifndef SFPSDLayer_H
#define SFPSDLayer_H

class MutableData;
class PSDLayer;

class SFPSDLayer
{
public:
#pragma mark - Effects Layer Informations
    SFPSDDropShadowEffectLayerInformation dropShadowEffectLayerInformation;
    SFPSDInnerShadowEffectLayerInformation innerShadowEffectLayerInformation;
    SFPSDOuterGlowEffectLayerInformation outerGlowEffectLayerInformation;
    SFPSDInnerGlowEffectLayerInformation innerGlowEffectLayerInformation;
    SFPSDBevelEffectLayerInformation bevelEffectLayerInformation;
    SFPSDSolidFillEffectLayerInformation solidFillEffectLayerInformation;
    EFloat2 documentSize;
    ///NSString *name;
    std::string mName;
	std::wstring mUniName;
    PSDLayer* psdlayer;
    float opacity;
    EFloat2 offset;
    int numberOfChannels;
    bool shouldFlipLayerData;
    bool shouldUnpremultiplyLayerData;
    const char *blendMode;
public:
    VRect imageRect();
    void writeEffectsLayerCommonState(MutableData *effectsLayerInformation);
    void writeEffectsLayerDropShadow(MutableData *effectsLayerInformation);
    void writeEffectsLayerInnerShadow(MutableData *effectsLayerInformation);
    void writeEffectsLayerOuterGlow(MutableData *effectsLayerInformation);
    void writeEffectsLayerInnerGlow(MutableData *effectsLayerInformation);
    void writeEffectsLayerBevel(MutableData *effectsLayerInformation);
    void writeEffectsLayerSolidFill(MutableData *effectsLayerInformation);
public:
    SFPSDLayer();
    SFPSDLayer(int numberOfChannels,
               float opacity,
               bool shouldFlipLayerData,
               bool shouldUnpremultiplyLayerData,
               const char *blendMode);
    virtual void writeLayerInfo(MutableData* result);
    virtual void writeLayerChannels(MutableData* result);
    virtual void layerChannels(std::vector<MutableData> &result);
    
    virtual void writeName(MutableData* result, int padding);
    virtual void writeEffectsLayer(MutableData *result);
    virtual void writeUnicodeName(MutableData *data);
    
    virtual void extraLayerInformation2(MutableData *extraDataStream);
};

#endif
