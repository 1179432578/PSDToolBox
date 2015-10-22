//
//  SFPSDWriter.m
//  SFPSDWriter
//
//  Created by Konstantin Erokhin on 06/06/13.
//  Copyright (c) 2013 Shiny Frog. All rights reserved.
//
//  Inspired by PSDWriter by Ben Gotow ( https://github.com/bengotow/PSDWriter )
//

#include "SFPSDWriter.h"
//#include "PNGLoader.h"

SFPSDWriter::SFPSDWriter(EFloat2 documentSize)
{
    float resolution = 72.0f;
    SFPSDResolutionUnit resolutionUnit = SFPSDResolutionUnitPPI;
    if (resolutionUnit == SFPSDResolutionUnitPPC) {
        // Converting the resolution to PPC
        resolution = resolution * 2.54;
    }
    
    mDocumentSize = documentSize;
    SFPSDResolution currentDocumentResolution;
    currentDocumentResolution.hResolution = resolution;
    currentDocumentResolution.vResolution = resolution;
    mDocumentResolution = currentDocumentResolution;
    mDocumentResolutionUnit = resolutionUnit;
    
    mColorProfile = SFPSDNoColorProfile;
    
    mPreviewGenerator = new PSDPreviewGenerator(documentSize.x, documentSize.y);
}
SFPSDWriter::SFPSDWriter(EFloat2 documentSize, float resolution, SFPSDResolutionUnit resolutionUnit)
{
    if (resolutionUnit == SFPSDResolutionUnitPPC) {
        // Converting the resolution to PPC
        resolution = resolution * 2.54;
    }
    
    mDocumentSize = documentSize;
    SFPSDResolution currentDocumentResolution;
    currentDocumentResolution.hResolution = resolution;
    currentDocumentResolution.vResolution = resolution;
    mDocumentResolution = currentDocumentResolution;
    mDocumentResolutionUnit = resolutionUnit;
    
    mColorProfile = SFPSDNoColorProfile;
    
    mPreviewGenerator = new PSDPreviewGenerator(documentSize.x, documentSize.y);
}
void SFPSDWriter::preprocess()
{
    // Closing all the eventual groups opened and not closed
    int groupsToClose = 0;
    for (size_t i = 0; i < mLayers.size(); i++) {
        SFPSDLayer *layer = mLayers[i];
        if (dynamic_cast<SFPSDGroupOpeningLayer*>(layer)) {
            ++groupsToClose;
        }
        else if (dynamic_cast<SFPSDGroupClosingLayer*>(layer)) {
            --groupsToClose;
        }
    }
    for (int i = 0; i < groupsToClose; i++) {
        closeCurrentGroupLayer();
    }
}

SFPSDLayer *SFPSDWriter::addLayerWithPSDLayer(PSDLayer* layer, const std::string& name, const std::wstring& uniName)
{
	return addLayerWithPSDLayer(layer, name, uniName, 1.0f, EFloat2(0.0f, 0.0f));
}
SFPSDLayer *SFPSDWriter::addLayerWithPSDLayer(PSDLayer* layer, const std::string& name, const std::wstring& uniName, float opacity, EFloat2 offset)
{
    SFPSDLayer* ret = new SFPSDLayer;
    
    ret->documentSize = mDocumentSize;
    ret->psdlayer = layer;
    ret->opacity = opacity;
    ret->mName = name;
	ret->mUniName = uniName;
    ret->offset = offset;
    ret->numberOfChannels = numberOfChannels();

    mLayers.push_back(ret);
    return ret;
}

SFPSDGroupOpeningLayer *SFPSDWriter::openGroupLayerWithName(const std::string& name, const std::wstring& uniName)
{
    return openGroupLayerWithName(name, uniName, 1.0, false);
}

SFPSDGroupOpeningLayer *SFPSDWriter::openGroupLayerWithName(const std::string& name, const std::wstring& uniName, float opacity, bool isOpened)
{
    SFPSDGroupOpeningLayer *layer = new SFPSDGroupOpeningLayer(name, uniName, opacity,isOpened );
    layer->documentSize = mDocumentSize;
    mLayers.push_back(layer);
    return layer;
}

SFPSDGroupClosingLayer *SFPSDWriter::closeCurrentGroupLayer()
{
    SFPSDGroupOpeningLayer *lastOpenedGroup = nullptr;
    int closedGroups = 0;
    std::vector<SFPSDLayer*>::reverse_iterator riter = mLayers.rbegin();
    std::vector<SFPSDLayer*>::reverse_iterator rend = mLayers.rend();
    for (; riter != rend; riter++) {
        SFPSDLayer* layer = *riter;

        if (dynamic_cast<SFPSDGroupClosingLayer*>(layer)) {
            ++closedGroups;
        }
        if (dynamic_cast<SFPSDGroupOpeningLayer*>(layer)) {
            if (closedGroups == 0) {
                lastOpenedGroup = (SFPSDGroupOpeningLayer *)layer;
                break;
            }
            else {
                --closedGroups;
            }
        }
    }
    
    if (nullptr == lastOpenedGroup) {
        
        return nullptr;
    }
    
    SFPSDGroupClosingLayer *layer = new SFPSDGroupClosingLayer;
    layer->documentSize = mDocumentSize;
    layer->groupOpeningLayer = lastOpenedGroup;
    
    mLayers.push_back(layer);
    
    return layer;
}

int SFPSDWriter::numberOfChannels()
{
    return 4;
}

// psd
MutableData* SFPSDWriter::createPSDData2()
{

    MutableData* result = new MutableData;
    //printf("max size %d", result->m_data.max_size());
    //result->m_data.resize(102400000);
    preprocess();
    
    // Write the File Header Section
    writeFileHeaderSection(result);//26
    
    // Write the Color Mode Data Section
    writeColorModeDataSection(result);//4
    
    // Write the Image Resources Section
    writeImageResourceSection(result);//46
    
    // Write the Layer and Mask Information Section
    writeLayerAndMaskInformationSection(result);
    
    // Write the Image Data Section
    writeImageDataSection(result);
    
    return result;
}

// FILE HEADER SECTION
// -----------------------------------------------------------------------------------------------

void SFPSDWriter::writeFileHeaderSection(MutableData*result)
{
    // size 26
    result->AppendUTF8String("8BPS", 4);
    result->AppendValue(1, 2);
    result->AppendValue(0, 6);
    result->AppendValue(numberOfChannels(), 2);
    result->AppendValue(mDocumentSize.y, 4);
    result->AppendValue(mDocumentSize.x, 4);
    
    result->AppendValue(8, 2);
    result->AppendValue(3, 2);
}

// COLOR MODE DATA SECTION
// -----------------------------------------------------------------------------------------------

void SFPSDWriter::writeColorModeDataSection(MutableData *result)
{
    // size 4
    result->AppendValue(0, 4);
}

// IMAGE RESOURCES SECTION
// -----------------------------------------------------------------------------------------------

void SFPSDWriter::writeImageResourceSection(MutableData *result)
{
    // size 46
    // write images resources section. This is used to store things like current layer.
    MutableDataPtr imageResources = new MutableData;
    
    // write the resolutionInfo structure. Don't have the definition for this, so we
    // have to just paste in the right bytes.
    imageResources->AppendUTF8String("8BIM", 4);
    ///[imageResources sfAppendValue:1005 length:2]; // 1005 - ResolutionInfo structure - See Appendix A in Photoshop API Guide.pdf
    imageResources->AppendValue(1005, 2);
    ///[imageResources sfAppendValue:0 length:2];
    imageResources->AppendValue(0, 2);
    ///[imageResources sfAppendValue:16 length:4];
    imageResources->AppendValue(16, 4);
    
    // Converting the resolution to a fixed point 16-binary digit number
    int hResolution = mDocumentResolution.hResolution * 65536.0 + 0.5;
    int vResolution = mDocumentResolution.vResolution * 65536.0 + 0.5;
    
    // write the current resolution info
    ///[imageResources sfAppendValue:hResolution length:4];                    // hRes - Horizontal resolution in pixels per inch
    imageResources->AppendValue(hResolution, 4);
    ///[imageResources sfAppendValue:self.documentResolutionUnit length:2];    // hResUnit - 1 = display horitzontal resolution in pixels per inch; 2 = display horitzontal resolution in pixels per cm
    imageResources->AppendValue(mDocumentResolutionUnit, 2);
    ///[imageResources sfAppendValue:1 length:2];                              // widthUnit - Display width as 1=inches; 2=cm; 3=points; 4=picas; 5=columns
    imageResources->AppendValue(1, 2);
    ///[imageResources sfAppendValue:vResolution length:4];                    // vRes -  Vertial resolution in pixels per inch
    imageResources->AppendValue(vResolution, 4);
    ///[imageResources sfAppendValue:self.documentResolutionUnit length:2];    // vResUnit - 1 = display horitzontal resolution in pixels per inch; 2 = display horitzontal resolution in pixels per cm
    imageResources->AppendValue(mDocumentResolutionUnit, 2);
    ///[imageResources sfAppendValue:1 length:2];                              // heightUnit - Display width as 1=inches; 2=cm; 3=points; 4=picas; 5=columns
    imageResources->AppendValue(1, 2);
    
    // write the current layer structure
    ///[imageResources sfAppendUTF8String:@"8BIM" length:4];
    imageResources->AppendUTF8String("8BIM", 4);
    ///[imageResources sfAppendValue:1024 length:2]; // 1024 - Layer state information - 2 bytes containing the index of target layer (0 = bottom layer)
    imageResources->AppendValue(1024, 2);
    ///[imageResources sfAppendValue:0 length:2];
    imageResources->AppendValue(0, 2);
    ///[imageResources sfAppendValue:2 length:4];
    imageResources->AppendValue(2, 4);
    ///[imageResources sfAppendValue:0 length:2]; // current layer = 0
    imageResources->AppendValue(0, 2);
    
    result->AppendValue(imageResources->Length(), 4);//4
    result->AppendData(imageResources.get());//42
}

// LAYER AND MASK INFORMATION SECTION
// -----------------------------------------------------------------------------------------------

void SFPSDWriter::writeLayerAndMaskInformationSection(MutableData *result)
{
    // layer and mask information section. contains basic data about each layer (its mask, its channels,
    // its layer effects, its annotations, transparency layers, wtf tons of shit.) We need to actually
    // create this.
    
    MutableData layerInfo;
    size_t layerCount = mLayers.size();
    
    // write the layer count
    layerInfo.AppendValue(layerCount, 2);
    
    // Writing the layer information for each layer
    for (size_t i = 0; i < mLayers.size(); i++) {
        SFPSDLayer *layer = mLayers[i];
        
        layer->writeLayerInfo(&layerInfo);

        //static int m = 0;
        //printf("writeLayerInfo  %d  size: %d \n", m++, mLayers.size());
    }
    

    // Writing the layer channels
    for (size_t i = 0; i < mLayers.size(); i++) {
        SFPSDLayer *layer = mLayers[i];
        
        layer->writeLayerChannels(&layerInfo);

        //static int m = 0;
        //printf("write  %d   LayerChannels  \n", m++);
    }

    //printf("done!");

    if (layerInfo.Length() % 2 != 0)
        layerInfo.AppendValue(0, 1);

    // write length of layer and mask information section
    result->AppendValue(layerInfo.Length()+4, 4);

    // write length of layer info
    result->AppendValue(layerInfo.Length(), 4);
        
    // write out actual layer info
    result->AppendData(&layerInfo);
}

// IMAGE DATA SECTION
// -----------------------------------------------------------------------------------------------

void SFPSDWriter::writeImageDataSection(MutableData *result)
{
    // write compression format = 1 = RLE
    result->AppendValue(1, 2);
    {
        
        mPreviewData = new MutableData;
        for (int y = 0; y < mPreviewGenerator->m_preview.height(); y++) {
            for (int x = 0; x < mPreviewGenerator->m_preview.width(); x++) {
                
                for (int c = 0; c < 4; c++) {
                    unsigned char p = mPreviewGenerator->m_preview.atXY(x, y, c);
                    mPreviewData->AppendBytes(&p, 1);
                    
                }
            }
        }
        
        MutableData byteCounts;
        MutableData scanlines;
        
        int imageRowBytes = mDocumentSize.x * 4;
        
        for (int channel = 0; channel < numberOfChannels(); channel++) {
            for (int row = 0; row < mDocumentSize.y; row++) {
                DataRange packRange = {row * imageRowBytes + channel, imageRowBytes};

                MutableData packed;
                mPreviewData->PackedBitsForRange(packRange, 4, &packed);
                
                byteCounts.AppendValue(packed.Length(), 2);
                scanlines.AppendData(&packed);
            }
        }
        
        
        // chop off the image data from the original file
     
        result->AppendData(&byteCounts);
        result->AppendData(&scanlines);
    }
}
