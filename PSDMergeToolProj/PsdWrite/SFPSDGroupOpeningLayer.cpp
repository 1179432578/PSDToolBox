//
//  SFPSDGroupOpeningLayer.m
//  SFPSDWriter
//
//  Created by Konstantin Erokhin on 06/06/13.
//  Copyright (c) 2013 Shiny Frog. All rights reserved.
//
//  Inspired by PSDWriter by Ben Gotow ( https://github.com/bengotow/PSDWriter )
//

#include "SFPSDGroupOpeningLayer.h"

#include "MutableData.h"

void SFPSDGroupOpeningLayer::extraLayerInformation2(MutableData *extraDataStream)
{
    //MutableData *extraDataStream = new MutableData;
    
    extraDataStream->AppendValue(0, 4); // Layer mask / adjustment layer data. Size of the data: 36, 20, or 0.
    extraDataStream->AppendValue(0, 4); // Layer blending ranges data. Length of layer blending ranges data
    
    // Temporally hanging the name to the default PS group's ending marker name
    std::string layerName = mName;
	std::wstring layerUniName = mUniName;

    mName = "</Layer group>";
	mUniName = L"</Layer group>";
    
    // Layer name: Pascal string, padded to a multiple of 4 bytes.
    writeName(extraDataStream, 4);
    
    // Section divider setting (Photoshop 6.0)
    extraDataStream->AppendUTF8String("8BIM", 4);
    extraDataStream->AppendUTF8String("lsct", 4);
    extraDataStream->AppendValue(4, 4); // Section divider length
    extraDataStream->AppendValue(3, 4); // Type. 0 = any other type of layer, 1 = open "folder", 2 = closed "folder", 3 = bounding section divider, hidden in the UI
    
    // Unicode layer name (Photoshop 5.0). Unicode string (4 bytes length + string).
    writeUnicodeName(extraDataStream);
    
    // Restoring the layer name
    mName = layerName;
	mUniName = layerUniName;
    
    //return extraDataStream;
}


