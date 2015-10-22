//
//  SFPSDWriter.h
//  SFPSDWriter
//
//  Created by Konstantin Erokhin on 06/06/13.
//  Copyright (c) 2013 Shiny Frog. All rights reserved.
//
//  Inspired by PSDWriter by Ben Gotow ( https://github.com/bengotow/PSDWriter )
//

#include "SFPSDLayer.h"
#include "SFPSDGroupOpeningLayer.h"
#include "SFPSDGroupClosingLayer.h"

#include "PSDPreviewGenerator.h"
#include "MutableData.h"
#include <vector>

#ifndef SFPSDWriter_H
#define SFPSDWriter_H

struct SFPSDResolution {
    float hResolution;
    float vResolution;
};
typedef struct SFPSDResolution SFPSDResolution;

enum SFPSDResolutionUnit {
    SFPSDResolutionUnitPPI = 1,
    SFPSDResolutionUnitPPC = 2
};
typedef enum SFPSDResolutionUnit SFPSDResolutionUnit;

enum SFPSDColorProfile {
    SFPSDNoColorProfile,            // No color profile (Don't Color Manage This Document)
    SFPSDGenericRGBColorProfile,    // Generic RGB
    SFPSDSRGBColorProfile,          // sRGB IEC61966-2.1
    SFPSDAdobeRGB1998ColorProfile   // Adobe RGB (1998)
};
typedef enum SFPSDColorProfile SFPSDColorProfile;

class SFPSDWriter
{
public:
    PSDPreviewGenerator* mPreviewGenerator;
    MutableDataPtr mPreviewData;
    std::vector<SFPSDLayer*> mLayers;
    EFloat2 mDocumentSize;
    SFPSDResolution mDocumentResolution;
    SFPSDResolutionUnit mDocumentResolutionUnit;
    SFPSDColorProfile mColorProfile;
    bool mHasTransparentLayers;
private:
    void preprocess();
    void writeFileHeaderSection(MutableData* result);
    void writeColorModeDataSection(MutableData *result);
    void writeImageResourceSection(MutableData *result);
    void writeLayerAndMaskInformationSection(MutableData *result);
    void writeImageDataSection(MutableData *result);
public:
    SFPSDWriter(EFloat2 documentSize);
    SFPSDWriter(EFloat2 documentSize, float resolution, SFPSDResolutionUnit resolutionUnit);
	SFPSDLayer *addLayerWithPSDLayer(PSDLayer* layer, const std::string& name, const std::wstring& uniName);
    SFPSDLayer *addLayerWithPSDLayer(PSDLayer* layer, const std::string& name, const std::wstring& uniName, float opacity, EFloat2 offset);
    
	SFPSDGroupOpeningLayer *openGroupLayerWithName(const std::string& name, const std::wstring& uniName);
	SFPSDGroupOpeningLayer *openGroupLayerWithName(const std::string& name, const std::wstring& uniName, float opacity, bool isOpened);
    SFPSDGroupClosingLayer *closeCurrentGroupLayer();
    int numberOfChannels();
    MutableData* createPSDData2();
};

#endif
