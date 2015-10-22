//
//  SFPSDGroupLayer.h
//  SFPSDWriter
//
//  Created by Konstantin Erokhin on 06/06/13.
//  Copyright (c) 2013 Shiny Frog. All rights reserved.
//

#include "SFPSDLayer.h"

#ifndef SFPSDGroupLayer_H
#define SFPSDGroupLayer_H

class SFPSDGroupLayer : public SFPSDLayer
{
public:
    bool isOpened;
public:
    SFPSDGroupLayer(const std::string& name, const std::wstring& uniName);
	SFPSDGroupLayer(const std::string& name, const std::wstring& uniName, float opacity, bool isOpened);
    virtual void copyGroupInformationFrom(SFPSDGroupLayer *layer);
    virtual void layerChannels(std::vector<MutableData> &result) override;
};

#endif

