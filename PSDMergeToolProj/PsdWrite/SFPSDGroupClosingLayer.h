//
//  SFPSDGroupClosingLayer.h
//  SFPSDWriter
//
//  Created by Konstantin Erokhin on 06/06/13.
//  Copyright (c) 2013 Shiny Frog. All rights reserved.
//
//  Inspired by PSDWriter by Ben Gotow ( https://github.com/bengotow/PSDWriter )
//

#include "SFPSDGroupLayer.h"
#include "SFPSDGroupOpeningLayer.h"

#ifndef SFPSDGroupClosingLayer_H
#define SFPSDGroupClosingLayer_H

class SFPSDGroupClosingLayer : public SFPSDGroupLayer
{
public:
    SFPSDGroupOpeningLayer *groupOpeningLayer;
public:
    SFPSDGroupClosingLayer()
    : SFPSDGroupLayer("", L"")
    {}
    void extraLayerInformation2(MutableData *extraDataStream) override;
};

#endif
