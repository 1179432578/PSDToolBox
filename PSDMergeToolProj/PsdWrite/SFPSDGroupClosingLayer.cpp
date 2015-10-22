//
//  SFPSDGroupClosingLayer.m
//  SFPSDWriter
//
//  Created by Konstantin Erokhin on 06/06/13.
//  Copyright (c) 2013 Shiny Frog. All rights reserved.
//
//  Inspired by PSDWriter by Ben Gotow ( https://github.com/bengotow/PSDWriter )
//

#include "SFPSDGroupClosingLayer.h"

#include "MutableData.h"


#pragma mark - Overrides of SFPSDLayer functions

void SFPSDGroupClosingLayer::extraLayerInformation2(MutableData *extraDataStream)
{
    //MutableData *extraDataStream = new MutableData;
    
    if (nullptr != groupOpeningLayer) {
        copyGroupInformationFrom(groupOpeningLayer);
    }
    
    extraDataStream->AppendValue(0, 4); // Layer mask / adjustment layer data. Size of the data: 36, 20, or 0.
    extraDataStream->AppendValue(0, 4); // Layer blending ranges data. Length of layer blending ranges data
    
    // Layer name: Pascal string, padded to a multiple of 4 bytes.
    writeName(extraDataStream, 4);
    
    // Section divider setting (Photoshop 6.0)
    extraDataStream->AppendUTF8String("8BIM", 4);
    extraDataStream->AppendUTF8String("lsct", 4);
    extraDataStream->AppendValue(12, 4); // Section divider length
    
    if (isOpened) {
        extraDataStream->AppendValue(1, 4); // Type. 0 = any other type of layer, 1 = open "folder", 2 = closed "folder", 3 = bounding section divider, hidden in the UI
    }
    else {
        extraDataStream->AppendValue(2, 4); // Type. 0 = any other type of layer, 1 = open "folder", 2 = closed "folder", 3 = bounding section divider, hidden in the UI
    }
    
    extraDataStream->AppendUTF8String("8BIM", 4);
    extraDataStream->AppendUTF8String(SFPSDLayerBlendModePassThrough, 4); // Blend mode: pass
    
    // Writing the Effects Layer containing information about Drop Shadow, Inner Shadow, Outer Glow, Inner Glow, Bevel, Solid Fill
    writeEffectsLayer(extraDataStream);
    
    // Unicode layer name (Photoshop 5.0). Unicode string (4 bytes length + string).
    writeUnicodeName(extraDataStream);
    
    //return extraDataStream;
}

