//
//  SFPSDGroupOpeningLayer.h
//  SFPSDWriter
//
//  Created by Konstantin Erokhin on 06/06/13.
//  Copyright (c) 2013 Shiny Frog. All rights reserved.
//
//  Inspired by PSDWriter by Ben Gotow ( https://github.com/bengotow/PSDWriter )
//

#include "SFPSDGroupLayer.h"

#ifndef SFPSDGroupOpeningLayer_H
#define SFPSDGroupOpeningLayer_H

class SFPSDGroupOpeningLayer : public SFPSDGroupLayer
{
public:
    SFPSDGroupOpeningLayer(const std::string& name, const std::wstring& uniName, float opacity, bool isOpened)
		: SFPSDGroupLayer(name, uniName, opacity, isOpened)
    {}
    void extraLayerInformation2(MutableData *extraDataStream) override;
};

#endif
