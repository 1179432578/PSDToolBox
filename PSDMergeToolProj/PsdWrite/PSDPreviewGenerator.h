//
//  PSDPreviewGenerator.h
//  psdtool
//
//  Created by xhnsworks on 5/6/15.
//  Copyright (c) 2015 徐 海宁. All rights reserved.
//

#ifndef __psdtool__PSDPreviewGenerator__
#define __psdtool__PSDPreviewGenerator__

#include <stdio.h>
#include "CImg.h"
class PSDLayer;
class PSDPreviewGenerator
{
public:
    cimg_library::CImg<unsigned char> m_preview;
public:
    PSDPreviewGenerator(int width, int height)
    : m_preview(width, height, 1, 4, 255)
    {}
    void Blit(PSDLayer* image,
              cimg_library::CImg<unsigned char>& result);
    void Blit(PSDLayer* image);
};

#endif /* defined(__psdtool__PSDPreviewGenerator__) */
