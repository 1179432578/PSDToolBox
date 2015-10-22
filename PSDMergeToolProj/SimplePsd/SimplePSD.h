//
//  SimplePSD.h
//  psdrw
//
//  Created by 鲁飞 on 15/9/21.
//  Copyright (c) 2015年 鲁飞. All rights reserved.
//

#ifndef __psdrw__SimplePSD__
#define __psdrw__SimplePSD__

#include <stdio.h>
#include "psdread.h"
#include "SFPSDWriter.h"
#include <string>
#include <vector>
#include <algorithm>

class Dialog;

class SimplePSD{
public:
    std::vector<PSDLayer*> m_forest;
    PSDLayer *m_newTree;
    SFPSDWriter *m_psdWrite;
    Dialog *dialog;

    
    SimplePSD();
    void createForest(std::vector<std::string> &psdFileNames);
    void markForest(std::string &PsdLayersPath, int psdID);
    void createNewTree();
    void writePsdByNewTree(std::string savefilePath);
    void writeNewTreeA(PSDLayer * layer);
    PSDLayer* findPsdLayer(std::string PsdLayersPath, int psdID);

};

#endif /* defined(__psdrw__SimplePSD__) */
