//
//  SFPSDGroupLayer.m
//  SFPSDWriter
//
//  Created by Konstantin Erokhin on 06/06/13.
//  Copyright (c) 2013 Shiny Frog. All rights reserved.
//

#include "SFPSDGroupLayer.h"

#include "MutableData.h"

SFPSDGroupLayer::SFPSDGroupLayer(const std::string& _name, const std::wstring& _uniName)
{
    mName = _name;
	mUniName = _uniName;
    isOpened = false;
    opacity = 1.0f;
}
SFPSDGroupLayer::SFPSDGroupLayer(const std::string& _name, const std::wstring& _uniName, float _opacity, bool _isOpened)
{
    mName = _name;
	mUniName = _uniName;
    isOpened = _isOpened;
    opacity = _opacity;
}


void SFPSDGroupLayer::copyGroupInformationFrom(SFPSDGroupLayer *layer)
{
    // Copying later informations
    ///[self setName:layer.name];
    ///[self setOpacity:layer.opacity];
    ///[self setIsOpened:layer.isOpened];
    mName = layer->mName;
	mUniName = layer->mUniName;
    opacity = layer->opacity;
    isOpened = layer->isOpened;
/**
    // Copying Effect Layers
    [self setDropShadowEffectLayerInformation:layer.dropShadowEffectLayerInformation];
    [self setInnerShadowEffectLayerInformation:layer.innerShadowEffectLayerInformation];
    [self setOuterGlowEffectLayerInformation:layer.outerGlowEffectLayerInformation];
    [self setInnerGlowEffectLayerInformation:layer.innerGlowEffectLayerInformation];
    [self setBevelEffectLayerInformation:layer.bevelEffectLayerInformation];
    [self setSolidFillEffectLayerInformation:layer.solidFillEffectLayerInformation];
 **/
    dropShadowEffectLayerInformation = layer->dropShadowEffectLayerInformation;
    innerShadowEffectLayerInformation = layer->innerShadowEffectLayerInformation;
    outerGlowEffectLayerInformation = layer->outerGlowEffectLayerInformation;
    innerGlowEffectLayerInformation = layer->innerGlowEffectLayerInformation;
    bevelEffectLayerInformation = layer->bevelEffectLayerInformation;
    solidFillEffectLayerInformation = layer->solidFillEffectLayerInformation;
}

#pragma mark - Overrides of SFPSDLayer functions
/**
- (NSArray *)layerChannels
{
    // Creating empty channels for the Group Layer with only compression formats
    NSMutableArray *layerChannels = [NSMutableArray array];
    for (int channel = 0; channel < [self numberOfChannels]; channel++) {
        NSMutableData *channelData = [NSMutableData data];
        // write channel compression format
        [channelData sfAppendValue:0 length:2];
        // add completed channel data to channels array
        [layerChannels addObject:channelData];
    }
    return layerChannels;
}
**/
void SFPSDGroupLayer::layerChannels(std::vector<MutableData> &result)
{

    for (int channel = 0; channel < numberOfChannels; channel++) {
        result.push_back(MutableData());
        // write channel compression format
        result[channel].AppendValue(0, 2);
        // add completed channel data to channels array
//        result.push_back(&channelData[channel]);
    }

}
