//
//  SFPSDLayer.m
//  SFPSDWriter
//
//  Created by Konstantin Erokhin on 06/06/13.
//  Copyright (c) 2013 Shiny Frog. All rights reserved.
//
//  Inspired by PSDWriter by Ben Gotow ( https://github.com/bengotow/PSDWriter )
//

#include "psdread.h"
#include "SFPSDLayer.h"
#include "MutableData.h"
#include <math.h>

#pragma mark - Init and dealloc

SFPSDLayer::SFPSDLayer()
{
    numberOfChannels = 4;
    opacity = 1.0f;
    shouldFlipLayerData = false;
    shouldUnpremultiplyLayerData = false;
    blendMode = SFPSDLayerBlendModeNormal;
    psdlayer = nullptr;
}

SFPSDLayer::SFPSDLayer(int _numberOfChannels,
                       float _opacity,
                       bool _shouldFlipLayerData,
                       bool _shouldUnpremultiplyLayerData,
                       const char *_blendMode)
{
    numberOfChannels = _numberOfChannels;
    opacity = _opacity;
    shouldFlipLayerData = _shouldFlipLayerData;
    shouldUnpremultiplyLayerData = _shouldUnpremultiplyLayerData;
    blendMode = _blendMode;
    psdlayer = nullptr;
}

VRect SFPSDLayer::imageRect()
{
    if (psdlayer) {
        VRect imageRect = VRect(offset.x, offset.y,
                                     psdlayer->GetWidth() + offset.x, psdlayer->GetHeight() + offset.y);
        return imageRect;
    }
    else {
        VRect imageRect = VRect(0, 0,
                                      0, 0);
        return imageRect;
    }
}

#pragma mark - Public writing functions


void SFPSDLayer::writeLayerInfo(MutableData*result)
{
    VRect bounds = imageRect();
    bounds.left = floorf(bounds.left);
    bounds.top = floorf(bounds.top);
    bounds.right = floorf(bounds.right);
    bounds.bottom = floorf(bounds.bottom);

    result->AppendValue((long)bounds.top, 4);
    result->AppendValue((long)bounds.left, 4);
    result->AppendValue((long)bounds.bottom, 4);
    result->AppendValue((long)bounds.right, 4);
    
    // print out number of channels in the layer
    result->AppendValue(numberOfChannels, 2);
    
    // ARC in this case not cleans the memory used for layerChannels even after the SFPSDWriter is cleared.
    // With an autoreleasepool we force the clean of the memory.
    {
        std::vector<MutableData> channels;
        layerChannels(channels);
        
        // print out data about each channel of the RGB
        for (int i = 0; i < 3; i++) {
            result->AppendValue(i, 2);
            int channelInformationLength = channels[i].Length();
            result->AppendValue(channelInformationLength, 4);
        }
        
        // If the alpha channel exists
        if (numberOfChannels > 3) {
            // The alpha channel is number -1
            unsigned char b[2] = {0xFF, 0xFF};
            result->AppendBytes(b, 2);
            int channelInformationLength = channels[3].Length();
            result->AppendValue(channelInformationLength, 4);
        }
//        for (size_t i = 0; i < channels.size(); i++) {
//            delete channels[i];
//        }
        // channels[0] is array head
        //delete [] channels[0];
    } // autoreleasepool
    
    // print out blend mode
    result->AppendUTF8String("8BIM", 4);
    result->AppendUTF8String(blendMode, 4);
    
    // print out opacity
    int o = ceilf(opacity * 255.0f);
    result->AppendValue(o, 1);
    
    // print out clipping
    result->AppendValue(0, 1);
    
    // print out flags.
    // bit 0 = transparency protected;
    // bit 1 = visible;
    // bit 2 = obsolete;
    // bit 3 = 1 for Photoshop 5.0 and later, tells if bit 4 has useful information;
    // bit 4 = pixel data irrelevant to appearance of document
    // (visible:false bit1=1)
    result->AppendValue(0, 1);
    
    // print out filler
    result->AppendValue(0, 1);
    
    // Overrided in special layers
    MutableData extraData;
    extraLayerInformation2(&extraData);
    
    // print out extra data length
    result->AppendValue(extraData.Length(), 4);
    // print out extra data
    result->AppendData(&extraData);
}

void SFPSDLayer::writeLayerChannels(MutableData*result)
{
    {
        std::vector<MutableData> channels;
        layerChannels(channels);

        //printf("start write layer channel\n");
        //float totalSize = 0.0f;
        MutableData* data;
        for (size_t i = 0; i < channels.size(); i++) {
            data = &channels[i];
            result->AppendData(data);
            //float size = data->m_data.size()/1000000.0f;
            //totalSize += size;
            //printf("writeLayer %d Channels: size:%fMB\n", i, size);
            //delete data;
        }
        //printf("writeLayerChannels total size:%fMB \n", totalSize);
        //printf("end write layer channel\n");

        // channels[0] is array head
        //delete [] channels[0];
    }
}

#pragma mark - Protecred functions [should never be used from outside the class]

void SFPSDLayer::layerChannels(std::vector<MutableData> &result)
{
    VRect bounds = imageRect();
    int layerWidth = (int)floorf(bounds.right - bounds.left);
    int layerHeight = (int)floorf(bounds.bottom - bounds.top);
    
    int imageRowBytes = layerWidth * 4;
    
    MutableData imageData;
    //printf("pos %d\n", (int)&imageData);
    psd_argb_color* pixels = psdlayer->GetPixels();
    for (int y = 0; y < psdlayer->GetHeight(); y++) {
        for (int x = 0; x < psdlayer->GetWidth(); x++) {
            unsigned char* p = (unsigned char*)&pixels[y * psdlayer->GetWidth() + x];
            for (int c = 0; c < 4; c++) {
                imageData.AppendBytes(&p[c], 1);
            }
        }
    }
    
    //MutableData* channelData = new MutableData[numberOfChannels];
    for (int channel = 0; channel < numberOfChannels; channel++)
    {
        MutableData byteCounts;
        MutableData scanlines;

        for (int row = 0; row < layerHeight; row++)
        {
            int byteCount = 0;
            
            // Appending the layer's image row

            DataRange packRange = {row * imageRowBytes + channel, imageRowBytes};

            MutableData packed;
            imageData.PackedBitsForRange(packRange, 4, &packed);

            scanlines.AppendData(&packed);

            byteCount += packed.Length();
            
            byteCounts.AppendValue(byteCount, 2);
        }
        
        result.push_back(MutableData());
        // write channel compression format
        //channelData[channel].AppendValue(1, 2);
        result[channel].AppendValue(1, 2);
        
        // write channel byte counts
        result[channel].AppendData(&byteCounts);
        // write channel scanlines
        result[channel].AppendData(&scanlines);
        
        // add completed channel data to channels array
        //result.push_back(&channelData[channel]);
    }

}

static size_t PascalStringLengthPaddedTo(const std::string& src, size_t paddingInterval)
{
    size_t paddingsIntervals = (int)ceilf((float)src.size() / (float)paddingInterval);
    return paddingsIntervals * paddingInterval;
}

static std::string paddedTo(const std::string& src, size_t paddingInterval)
{
    std::string ret = src;
    size_t p = PascalStringLengthPaddedTo(src, paddingInterval);
    size_t m = ret.size();
    ret.resize(p);
    for (size_t i = m; i < ret.size(); i++) {
        ret[i] = 0x00;
    }
    return ret;
}

void SFPSDLayer::writeName(MutableData *result, int padding)
{
    std::string layerName = mName;
    layerName += " ";
    std::string tmp = paddedTo(layerName, 4);
    size_t strPascalNameLength = PascalStringLengthPaddedTo(tmp, 4);
    result->AppendValue(mName.size(), 1);
    result->AppendBytes((unsigned char*)tmp.c_str(), (int)strPascalNameLength - 1); // -1 because it was the space reserved for writing the heading length of the string
}

#pragma mark EXTRA LAYER INFORMATION

void SFPSDLayer::writeEffectsLayerCommonState(MutableData *effectsLayerInformation)
{
    // DESCRIPTION: Signature
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    // DESCRIPTION: Effects signatures: OSType key for which effects type to use: common state
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String(SFPSDEffectsLayerEffectSignatureCommonState, 4);
    // DESCRIPTION: Size of next three items: 7
    // LENGTH: 4
    effectsLayerInformation->AppendValue(7, 4);
    // DESCRIPTION: Version: 0
    // LENGTH: 4
    effectsLayerInformation->AppendValue(0, 4);
    // DESCRIPTION: Visible: always true
    // LENGTH: 1
    effectsLayerInformation->AppendValue(1, 1);
    // DESCRIPTION: Unused: always 0
    // LENGTH: 2
    effectsLayerInformation->AppendValue(0, 2);
}

void SFPSDLayer::writeEffectsLayerDropShadow(MutableData *effectsLayerInformation)
{
    SFPSDDropShadowEffectLayerInformation& currentDropShadow = dropShadowEffectLayerInformation;
    // DESCRIPTION: Signature
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    // DESCRIPTION: Effects signatures: OSType key for which effects type to use: drop shadow
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String(SFPSDEffectsLayerEffectSignatureDropShadow, 4);
    // DESCRIPTION: Size of the remaining items: 41 or 51 (depending on version)
    // LENGTH: 4
    effectsLayerInformation->AppendValue(51, 4);
    // DESCRIPTION: Version: 0 ( Photoshop 5.0) or 2 ( Photoshop 5.5)
    // LENGTH: 4
    effectsLayerInformation->AppendValue(2, 4);
    // DESCRIPTION: Blur value in pixels
    // LENGTH: 4
    // NOTES: I think the real length is 2
    //        "Size" value in Photoshop (0...250) PX
    effectsLayerInformation->AppendValue(currentDropShadow.getSize(), 2);
    // DESCRIPTION: Intensity as a percent
    // LENGTH: 4
    // NOTES: Not used in Photoshop
    effectsLayerInformation->AppendValue(0, 4);
    // DESCRIPTION: Angle in degrees
    // LENGTH: 4
    // NOTES: "Angle" in Photoshop (-360...360)
    effectsLayerInformation->AppendValue(currentDropShadow.getAngle(), 4);
    // DESCRIPTION: Distance in pixels
    // LENGTH: 4
    // NOTES: "Distance" in Photoshop (0...30000) PX
    effectsLayerInformation->AppendValue(currentDropShadow.getDistance(), 4);
    // DESCRIPTION: Not documented
    // LENGTH: -
    // NOTES: I think here are the missing 2 bytes from the blur value
    effectsLayerInformation->AppendValue(0, 2);
    // DESCRIPTION: Color: 2 bytes for space followed by 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    ///effectsLayerInformation->AppendCGColorRef:currentDropShadow.color length:10];
    
    effectsLayerInformation->AppendColor(currentDropShadow.color, 10);
    // DESCRIPTION: Blend mode: 4 bytes for signature and 4 bytes for key
    // LENGTH: 8
    // NOTES: "Blend Mode" in Photoshop
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    effectsLayerInformation->AppendUTF8String(currentDropShadow.blendMode, 4);
    // DESCRIPTION: Effect enabled
    // LENGTH: 1
    effectsLayerInformation->AppendValue(currentDropShadow.enabled, 1);
    // DESCRIPTION: Use this angle in all of the layer effects
    // LENGTH: 1
    // NOTES: "Use Global Light" in Photoshop
    effectsLayerInformation->AppendValue(currentDropShadow.useGlobalLight, 1);
    // DESCRIPTION: Opacity as a percent
    // LENGTH: 1
    // NOTES: (0...255)
    effectsLayerInformation->AppendValue(currentDropShadow.opacity255(), 1);
    // DESCRIPTION: (Version 2 only) Native color: 2 bytes for space followed by 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    effectsLayerInformation->AppendColor(currentDropShadow.color, 10);
}

void SFPSDLayer::writeEffectsLayerInnerShadow(MutableData *effectsLayerInformation)
{
    SFPSDInnerShadowEffectLayerInformation& currentInnerShadow = innerShadowEffectLayerInformation;
    
    // DESCRIPTION: Signature
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    // DESCRIPTION: Effects signatures: OSType key for which effects type to use: inner shadow
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String(SFPSDEffectsLayerEffectSignatureInnerShadow, 4);
    // DESCRIPTION: Size of the remaining items: 41 or 51 (depending on version)
    // LENGTH: 4
    effectsLayerInformation->AppendValue(51, 4);
    // DESCRIPTION: Version: 0 ( Photoshop 5.0) or 2 ( Photoshop 5.5)
    // LENGTH: 4
    effectsLayerInformation->AppendValue(2, 4);
    // DESCRIPTION: Blur value in pixels
    // LENGTH: 4
    // NOTES: I think the real length is 2
    //        "Size" value in Photoshop (0...250) PX
    effectsLayerInformation->AppendValue(currentInnerShadow.getSize(), 2);
    // DESCRIPTION: Intensity as a percent
    // LENGTH: 4
    // NOTES: Not used in Photoshop
    effectsLayerInformation->AppendValue(0, 4);
    // DESCRIPTION: Angle in degrees
    // LENGTH: 4
    // NOTES: "Angle" in Photoshop (-360...360)
    effectsLayerInformation->AppendValue(currentInnerShadow.getAngle(), 4);
    // DESCRIPTION: Distance in pixels
    // LENGTH: 4
    // NOTES: "Distance" in Photoshop (0...30000) PX
    effectsLayerInformation->AppendValue(currentInnerShadow.getDistance(), 4);
    // DESCRIPTION: Not documented
    // LENGTH: -
    // NOTES: I think here are the missing 2 bytes from the blur value
    effectsLayerInformation->AppendValue(0, 2);
    // DESCRIPTION: Color: 2 bytes for space followed by 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    
    effectsLayerInformation->AppendColor(currentInnerShadow.color, 10);
    // DESCRIPTION: Blend mode: 4 bytes for signature and 4 bytes for key
    // LENGTH: 8
    // NOTES: "Blend Mode" in Photoshop
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    effectsLayerInformation->AppendUTF8String(SFPSDLayerBlendModeNormal, 4);
    // DESCRIPTION: Effect enabled
    // LENGTH: 1
    effectsLayerInformation->AppendValue(currentInnerShadow.enabled, 1);
    // DESCRIPTION: Use this angle in all of the layer effects
    // LENGTH: 1
    // NOTES: "Use Global Light" in Photoshop
    effectsLayerInformation->AppendValue(currentInnerShadow.useGlobalLight, 1);
    // DESCRIPTION: Opacity as a percent
    // LENGTH: 1
    // NOTES: (0...255)
    effectsLayerInformation->AppendValue(currentInnerShadow.opacity255(), 1);
    // DESCRIPTION: (Version 2 only) Native color: 2 bytes for space followed by 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    effectsLayerInformation->AppendColor(currentInnerShadow.color, 10);
}

void SFPSDLayer::writeEffectsLayerOuterGlow(MutableData *effectsLayerInformation)
{
    /**
    SFPSDOuterGlowEffectLayerInformation *currentOuterGlow;
    if (nil != [self outerGlowEffectLayerInformation]) {
        currentOuterGlow = [self outerGlowEffectLayerInformation];
    }
    else {
        // Default values and not enabled
        currentOuterGlow = [[SFPSDOuterGlowEffectLayerInformation alloc] init];
    }
     **/
    SFPSDOuterGlowEffectLayerInformation& currentOuterGlow = outerGlowEffectLayerInformation;
    
    // DESCRIPTION: Signature
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    // DESCRIPTION: Effects signatures: OSType key for which effects type to use: outer glow
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String(SFPSDEffectsLayerEffectSignatureOuterGlow, 4);
    // DESCRIPTION: Size of the remaining items: 32 for Photoshop 5.0; 42 for 5.5
    // LENGTH: 4
    effectsLayerInformation->AppendValue(42, 4);
    // DESCRIPTION: Version: 0 for Photoshop 5.0; 2 for 5.5
    // LENGTH: 4
    effectsLayerInformation->AppendValue(2, 4);
    // DESCRIPTION: Blur value in pixels
    // LENGTH: 4
    // NOTES: I think the real length is 2
    //        "Size" value in Photoshop (0...250) PX
    effectsLayerInformation->AppendValue(currentOuterGlow.size, 2);
    // DESCRIPTION: Intensity as a percent
    // LENGTH: 4
    // NOTES: Not used in Photoshop
    effectsLayerInformation->AppendValue(0, 4);
    // DESCRIPTION: Not documented
    // LENGTH: -
    // NOTES: I think here are the missing 2 bytes from the blur value
    effectsLayerInformation->AppendValue(0, 2);
    // DESCRIPTION: Color: 2 bytes for space followed by 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    
    effectsLayerInformation->AppendColor(currentOuterGlow.color, 10);
    // DESCRIPTION: Blend mode: 4 bytes for signature and 4 bytes for the key
    // LENGTH: 8
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    effectsLayerInformation->AppendUTF8String(currentOuterGlow.blendMode, 4);
    // DESCRIPTION: Effect enabled
    // LENGTH: 1
    effectsLayerInformation->AppendValue(currentOuterGlow.enabled, 1);
    // DESCRIPTION: Opacity as a percent
    // LENGTH: 1
    // NOTES: (0...255)
    effectsLayerInformation->AppendValue(currentOuterGlow.opacity255(), 1);
    // DESCRIPTION: (Version 2 only) Native color space. 2 bytes for space followed by 4 * 2 byte color component
    // LENGTH: 4
    // NOTES: Has no alpha component
    effectsLayerInformation->AppendColor(currentOuterGlow.color, 10);
}

void SFPSDLayer::writeEffectsLayerInnerGlow(MutableData *effectsLayerInformation)
{
    /**
    SFPSDInnerGlowEffectLayerInformation *currentInnerGlow;
    if (nil != [self innerGlowEffectLayerInformation]) {
        currentInnerGlow = [self innerGlowEffectLayerInformation];
    }
    else {
        // Default values and not enabled
        currentInnerGlow = [[SFPSDInnerGlowEffectLayerInformation alloc] init];
    }
    **/
    SFPSDInnerGlowEffectLayerInformation &currentInnerGlow = innerGlowEffectLayerInformation;
    
    // DESCRIPTION: Signature
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    // DESCRIPTION: Effects signatures: OSType key for which effects type to use: inner glow
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String(SFPSDEffectsLayerEffectSignatureInnerGlow, 4);
    // DESCRIPTION: Size of the remaining items: 33 for Photoshop 5.0; 43 for 5.5
    // LENGTH: 4
    effectsLayerInformation->AppendValue(43, 4);
    // DESCRIPTION: Version: 0 for Photoshop 5.0; 2 for 5.5
    // LENGTH: 4
    effectsLayerInformation->AppendValue(2, 4);
    // DESCRIPTION: Blur value in pixels
    // LENGTH: 4
    // NOTES: I think the real length is 2
    //        "Size" value in Photoshop (0...250) PX
    effectsLayerInformation->AppendValue(currentInnerGlow.getSize(), 2);
    // DESCRIPTION: Intensity as a percent
    // LENGTH: 4
    // NOTES: Not used in Photoshop
    effectsLayerInformation->AppendValue(0, 4);
    // DESCRIPTION: Not documented
    // LENGTH: -
    // NOTES: I think here are the missing 2 bytes from the blur value
    effectsLayerInformation->AppendValue(0, 2);
    // DESCRIPTION: Color: 2 bytes for space followed by 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    
    effectsLayerInformation->AppendColor(currentInnerGlow.color, 10);
    // DESCRIPTION: Blend mode: 4 bytes for signature and 4 bytes for the key
    // LENGTH: 8
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    effectsLayerInformation->AppendUTF8String(currentInnerGlow.blendMode, 4);
    // DESCRIPTION: Effect enabled
    // LENGTH: 1
    effectsLayerInformation->AppendValue(currentInnerGlow.enabled, 1);
    // DESCRIPTION: Opacity as a percent
    // LENGTH: 1
    // NOTES: (0...255)
    effectsLayerInformation->AppendValue(currentInnerGlow.opacity255(), 1);
    // DESCRIPTION: (Version 2 only) Invert
    // LENGTH: 1
    // NOTES: "Source" value in Photoshop -> 0: Center | 1: Edge
    effectsLayerInformation->AppendValue(currentInnerGlow.source, 1);
    // DESCRIPTION: (Version 2 only) Native color space. 2 bytes for space followed by 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    effectsLayerInformation->AppendColor(currentInnerGlow.color, 10);
}

void SFPSDLayer::writeEffectsLayerBevel(MutableData *effectsLayerInformation)
{
    /**
    SFPSDBevelEffectLayerInformation *currentBevel;
    if (nil != [self bevelEffectLayerInformation]) {
        currentBevel = [self bevelEffectLayerInformation];
    }
    else {
        // Default values and not enabled
        currentBevel = [[SFPSDBevelEffectLayerInformation alloc] init];
    }
     **/
    SFPSDBevelEffectLayerInformation &currentBevel = bevelEffectLayerInformation;
    
    // DESCRIPTION: Signature
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    // DESCRIPTION: Effects signatures: OSType key for which effects type to use: bevel
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String(SFPSDEffectsLayerEffectSignatureBevel, 4);
    // DESCRIPTION: Size of the remaining items (58 for version 0, 78 for version 20
    // LENGTH: 4
    effectsLayerInformation->AppendValue(78, 4);
    // DESCRIPTION: Version: 0 for Photoshop 5.0; 2 for 5.5
    // LENGTH: 4
    effectsLayerInformation->AppendValue(2, 4);
    // DESCRIPTION: Angle in degrees
    // LENGTH: 4
    // NOTES: I think the real length is 2
    effectsLayerInformation->AppendValue(currentBevel.getAngle(), 2);
    // DESCRIPTION: Strength. Depth in pixels
    // LENGTH: 4
    // NOTES: Not used in Photoshop
    //        (1...250)
    effectsLayerInformation->AppendValue(1, 4);
    // DESCRIPTION: Blur value in pixels.
    // LENGTH: 4
    // NOTES: "Size" value in Photoshop (0...250) PX
    //        The behaviour of this value is very strange. For example with the Emboss style it is doubled inside Photoshop
    //        Furthermore the "Depth" Photoshop value (which we apparently have no access to) seems to be influenced by
    //        this size
    effectsLayerInformation->AppendValue(currentBevel.getSize(), 4);
    // DESCRIPTION: Not documented
    // LENGTH: -
    // NOTES: I think here are the missing 2 bytes from the angle value
    effectsLayerInformation->AppendValue(0, 2);
    // DESCRIPTION: Highlight blend mode: 4 bytes for signature and 4 bytes for the key
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    effectsLayerInformation->AppendUTF8String(currentBevel.highlightBlendMode, 4);
    // DESCRIPTION: Shadow blend mode: 4 bytes for signature and 4 bytes for the key
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    effectsLayerInformation->AppendUTF8String(currentBevel.shadowBlendMode, 4);
    // DESCRIPTION: Highlight color: 2 bytes for space followed by 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    
    effectsLayerInformation->AppendColor(currentBevel.highlightColor, 10);
    // DESCRIPTION: Shadow color: 2 bytes for space followed by 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    effectsLayerInformation->AppendColor(currentBevel.shadowColor, 10);
    // DESCRIPTION: Bevel style
    // LENGTH: 1
    // NOTES: 1: Outer Bevel | 2: Inner Bevel | 3: Emboss | 4: Pillow Emboss | 5: Stroke Emboss
    effectsLayerInformation->AppendValue(currentBevel.style, 1);
    // DESCRIPTION: Hightlight opacity as a percent
    // LENGTH: 1
    // NOTES: (0...255)
    effectsLayerInformation->AppendValue(currentBevel.highlightOpacity255(), 1);
    // DESCRIPTION: Shadow opacity as a percent
    // LENGTH: 1
    // NOTES:(0...255)
    effectsLayerInformation->AppendValue(currentBevel.shadowOpacity255(), 1);
    // DESCRIPTION: Effect enabled
    // LENGTH: 1
    effectsLayerInformation->AppendValue(currentBevel.enabled, 1);
    // DESCRIPTION: Use this angle in all of the layer
    // LENGTH: 1
    // NOTES: "Use Global Light" in Photoshop
    effectsLayerInformation->AppendValue(currentBevel.useGlobalLight, 1);
    // DESCRIPTION: Up or down
    // LENGTH: 1
    // NOTES: "Direction" value in Photoshop (1: Down, 2: Up)
    effectsLayerInformation->AppendValue(currentBevel.direction, 1);
    // DESCRIPTION: Real highlight color: 2 bytes for space; 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    //        Not used in Photoshop
    effectsLayerInformation->AppendColor(currentBevel.highlightColor, 10);
    // DESCRIPTION: Real shadow color: 2 bytes for space; 4 * 2 byte color component
    // LENGTH: 10
    // NOTES: Has no alpha component
    //        Not used in Photoshop
    effectsLayerInformation->AppendColor(currentBevel.shadowColor, 10);
}

void SFPSDLayer::writeEffectsLayerSolidFill(MutableData *effectsLayerInformation)
{
    /**
    SFPSDSolidFillEffectLayerInformation *currentSolidFill;
    if (nil != [self solidFillEffectLayerInformation]) {
        currentSolidFill = [self solidFillEffectLayerInformation];
    }
    else {
        // Default values and not enabled
        currentSolidFill = [[SFPSDSolidFillEffectLayerInformation alloc] init];
    }
     **/
    SFPSDSolidFillEffectLayerInformation &currentSolidFill = solidFillEffectLayerInformation;
    
    // DESCRIPTION: Signature
    // LENGTH: 4
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    // DESCRIPTION: Effects signatures: OSType key for which effects type to use: solid fill
    // LENGTH: 4
    // NOTES:
    effectsLayerInformation->AppendUTF8String(SFPSDEffectsLayerEffectSignatureSolidFill, 4);
    // DESCRIPTION: Size: 34
    // LENGTH: 4
    effectsLayerInformation->AppendValue(34, 4);
    // DESCRIPTION: Version: 2
    // LENGTH: 4
    effectsLayerInformation->AppendValue(2, 4);
    // DESCRIPTION: Key for blend mode
    // LENGTH: 4
    // NOTES: I think it is the blending mode 4 + 4
    effectsLayerInformation->AppendUTF8String("8BIM", 4);
    effectsLayerInformation->AppendUTF8String(currentSolidFill.blendMode, 4);
    // DESCRIPTION: Color space
    // LENGTH: 10
    // NOTES: Has no alpha component
    //        Seems to be the fill collor

    effectsLayerInformation->AppendColor(currentSolidFill.color, 10);
    // DESCRIPTION: Opacity
    // LENGTH: 1
    // NOTES: (0...255)
    effectsLayerInformation->AppendValue(currentSolidFill.opacity255(), 1);
    // DESCRIPTION: Enabled
    // LENGTH: 1
    effectsLayerInformation->AppendValue(currentSolidFill.enabled, 1);
    // DESCRIPTION: Native color space
    // LENGTH: 4
    // NOTES:
    effectsLayerInformation->AppendColor(currentSolidFill.color, 10);
}

void SFPSDLayer::writeEffectsLayer(MutableData *data)
{
    /**
    // We'll include the effects layer informations only if there is at least one effect enabled
    if ((nil == [self dropShadowEffectLayerInformation] || ![self dropShadowEffectLayerInformation].enabled) &&
        (nil == [self innerShadowEffectLayerInformation] || ![self innerShadowEffectLayerInformation].enabled) &&
        (nil == [self outerGlowEffectLayerInformation] || ![self outerGlowEffectLayerInformation].enabled) &&
        (nil == [self innerGlowEffectLayerInformation] || ![self innerGlowEffectLayerInformation].enabled) &&
        (nil == [self bevelEffectLayerInformation] || ![self bevelEffectLayerInformation].enabled) &&
        (nil == [self solidFillEffectLayerInformation] || ![self solidFillEffectLayerInformation].enabled)) {
        return;
    }
    **/
    // Temporary data container used to calculate the Effects Layer length before writing it on the data
    MutableData effectsLayerInformation;
    
    // DESCRIPTION: Version: 0
    // LENGTH: 2
    effectsLayerInformation.AppendValue(0, 2);
    // DESCRIPTION: Effects count: may be 6 (for the 6 effects in Photoshop 5 and 6) or 7 (for Photoshop 7.0)
    // LENGTH: 2
    effectsLayerInformation.AppendValue(7, 2);
    
    // COMMON STATE
    // ------------------------------------------------------------------------
    writeEffectsLayerCommonState(&effectsLayerInformation);
    
    // DROP SWADOW
    // ------------------------------------------------------------------------
    writeEffectsLayerDropShadow(&effectsLayerInformation);
    
    // INNER SWADOW
    // ------------------------------------------------------------------------
    writeEffectsLayerInnerShadow(&effectsLayerInformation);
    
    // OUTER GLOW
    // ------------------------------------------------------------------------
    writeEffectsLayerOuterGlow(&effectsLayerInformation);
    
    // INNER GLOW
    // ------------------------------------------------------------------------
    writeEffectsLayerInnerGlow(&effectsLayerInformation);
    
    // BEVEL
    // ------------------------------------------------------------------------
    writeEffectsLayerBevel(&effectsLayerInformation);
    
    // SOLID FILL
    // ------------------------------------------------------------------------
    writeEffectsLayerSolidFill(&effectsLayerInformation);
    
    // Appending to the data
    // ------------------------------------------------------------------------
    
    // round to length divisible by 2.
    // Requested by the spect but seems to be useless
    if (effectsLayerInformation.Length() % 2 != 0)
        effectsLayerInformation.AppendValue(0, 1);
    
    
    // DESCRIPTION: Signature: '8BIM' or '8B64'
    // LENGTH: 4
    data->AppendUTF8String("8BIM", 4);
    // DESCRIPTION: Key: a 4-character code (See individual sections)
    // LENGTH: 4
    // NOTES: Effects Layer (Photoshop 5.0)
    //        The key for the effects layer is 'lrFX'.
    data->AppendUTF8String("lrFX", 4);
    // DESCRIPTION: Length data below, rounded up to an even byte count.
    // LENGTH: 4
    data->AppendValue(effectsLayerInformation.Length(), 4);
    // DESCRIPTION: Data
    // LENGTH: Variable
    data->AppendData(&effectsLayerInformation);
}

void SFPSDLayer::writeUnicodeName(MutableData *data)
{
    data->AppendUTF8String("8BIM", 4);
    data->AppendUTF8String("luni", 4); // Unicode layer name (Photoshop 5.0)
    
    data->AppendValue((mUniName.size() * 2) + 4, 4); // length of the next bit of data
	data->AppendValue(mUniName.size(), 4); // length of the unicode string data
    
	const wchar_t* str = mUniName.c_str();
	for (size_t i = 0; i < mUniName.size(); i++) {
        data->AppendValue(str[i], 2);
    }
}

void SFPSDLayer::extraLayerInformation2(MutableData *extraDataStream)
{
    // new stream of data for the extra information
    //MutableData *extraDataStream = new MutableData;
    
    extraDataStream->AppendValue(0, 4); // Layer mask / adjustment layer data. Size of the data: 36, 20, or 0.
    
    extraDataStream->AppendValue(0, 4); // Layer blending ranges data. Length of layer blending ranges data
    
    // Layer name: Pascal string, padded to a multiple of 4 bytes.
    writeName(extraDataStream, 4);
    
    // Writing the Effects Layer containing information about Drop Shadow, Inner Shadow, Outer Glow, Inner Glow, Bevel, Solid Fill
    writeEffectsLayer(extraDataStream);
    
    // Unicode layer name (Photoshop 5.0). Unicode string (4 bytes length + string).
    writeUnicodeName(extraDataStream);
    
    //return extraDataStream;
}

