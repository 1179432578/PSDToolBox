//
//  PSDPreviewGenerator.cpp
//  psdtool
//
//  Created by xhnsworks on 5/6/15.
//  Copyright (c) 2015 徐 海宁. All rights reserved.
//

#include "PSDPreviewGenerator.h"
#include <vector>
#include "sfloat4.h"
#include "sint4.h"
#include "psdread.h"

void PSDPreviewGenerator::Blit(PSDLayer* image,
                               cimg_library::CImg<unsigned char>& result)
{
    psd_int left = image->GetLeft();  
    psd_int right = image->GetRight();
    psd_int top = image->GetTop();     
    psd_int width = right - left;
    right = left + width;
    
    psd_int xOffsetRelativeSource = 0;
    psd_int yOffsetRelativeSource = 0;
    psd_int xOffsetRelativeDestination = left;
    psd_int yOffserRelativeDestination = top;
  
	psd_argb_color* source = image->GetPixels();
    
    sfloat4 f255 = SFloat4(255.0f);
    sfloat4 fd255 = SFloat4(1.0f / 255.0f);
    sfloat4 one = SFloat4(1.0f);
    
    auto alphaBlendProc = [f255,
                           fd255,
                           one](unsigned char* pixel,
                                unsigned char& r,
                                unsigned char& g,
                                unsigned char& b,
                                unsigned char& a) {
        if (a != 255) {
            sint4 tmp0 = SInt4(pixel[0], pixel[1], pixel[2], pixel[3]);
            sfloat4 srcRGBA = SInt4_convert_ft4(tmp0);
            sint4 tmp1 = SInt4(r, g, b, a);
            sfloat4 dstRGBA = SInt4_convert_ft4(tmp1);
            srcRGBA = SFloat4_mul(srcRGBA, fd255);
            dstRGBA = SFloat4_mul(dstRGBA, fd255);
            
            sfloat4 srcRGB = SFloat4_assign(SFloat4_xyz(srcRGBA), 0.0f);
            sfloat4 dstRGB = SFloat4_assign(SFloat4_xyz(dstRGBA), 0.0f);
            
            sfloat4 srcAlpha = SFloat4_wwww(srcRGBA);
            sfloat4 dstAlpha = SFloat4_wwww(dstRGBA);
            
            sfloat4 outAlpha = SFloat4_add(srcAlpha, SFloat4_mul(dstAlpha, SFloat4_sub(one, srcAlpha)));
            
            if (SFloat4_get_x(&outAlpha) > 0.0f) {
                sfloat4 outRGB = SFloat4_div(SFloat4_add(
                                                         SFloat4_mul(srcRGB, srcAlpha),
                                                         SFloat4_mul(SFloat4_mul(dstRGB, dstAlpha), SFloat4_sub(one, srcAlpha))
                                                         ), outAlpha);
                
                sfloat4 outRGBA = SFloat4_add(outRGB, outAlpha);
                outRGBA = SFloat4_mul(outRGBA, f255);
                
                sint4 iOutRGBA = SFloat4_convert_int4(outRGBA);
                r = SInt4_get_x(&iOutRGBA);
                g = SInt4_get_y(&iOutRGBA);
                b = SInt4_get_z(&iOutRGBA);
                a = SInt4_get_w(&iOutRGBA);
            }
            else {
                r = 0;
                g = 0;
                b = 0;
                a = 0;
            }
        }
        else {
            sint4 tmp0 = SInt4(pixel[0], pixel[1], pixel[2], pixel[3]);
            sfloat4 srcRGBA = SInt4_convert_ft4(tmp0);
            sint4 tmp1 = SInt4(r, g, b, a);
            sfloat4 dstRGBA = SInt4_convert_ft4(tmp1);
            srcRGBA = SFloat4_mul(srcRGBA, fd255);
            dstRGBA = SFloat4_mul(dstRGBA, fd255);
            
            sfloat4 srcRGB = SFloat4_assign(SFloat4_xyz(srcRGBA), 0.0f);
            sfloat4 dstRGB = SFloat4_assign(SFloat4_xyz(dstRGBA), 0.0f);
            
            sfloat4 srcAlpha = SFloat4_wwww(srcRGBA);
            
            sfloat4 outRGB = SFloat4_add( SFloat4_mul(srcRGB, srcAlpha), SFloat4_mul(dstRGB, SFloat4_sub(one, srcAlpha)) );
            
            outRGB = SFloat4_mul(outRGB, f255);
            
            sint4 iOutRGB = SFloat4_convert_int4(outRGB);
            r = SInt4_get_x(&iOutRGB);
            g = SInt4_get_y(&iOutRGB);
            b = SInt4_get_z(&iOutRGB);
            a = 255;
        }
    };
    
    for (psd_int rowCount = 0; rowCount < image->GetHeight(); rowCount++) {
        if (rowCount + yOffserRelativeDestination < 0)
            continue;
        if (rowCount + yOffserRelativeDestination >= result.height())
            break;
        for (psd_int rowPixelCount = 0; rowPixelCount < image->GetWidth(); rowPixelCount++) {
            if (rowPixelCount + xOffsetRelativeDestination < 0)
                continue;
            if (rowPixelCount + xOffsetRelativeDestination >= result.width())
                break;
            
            unsigned char* pixel = (unsigned char*)&source[ (rowCount + yOffsetRelativeSource) * image->GetWidth() + (rowPixelCount + xOffsetRelativeSource) ];
            unsigned char& r = result.atXY(int(rowPixelCount + xOffsetRelativeDestination), int(rowCount + yOffserRelativeDestination), 0);
            unsigned char& g = result.atXY(int(rowPixelCount + xOffsetRelativeDestination), int(rowCount + yOffserRelativeDestination), 1);
            unsigned char& b = result.atXY(int(rowPixelCount + xOffsetRelativeDestination), int(rowCount + yOffserRelativeDestination), 2);
            unsigned char& a = result.atXY(int(rowPixelCount + xOffsetRelativeDestination), int(rowCount + yOffserRelativeDestination), 3);
            
            alphaBlendProc(pixel, r, g, b, a);
        }
    }
}

void PSDPreviewGenerator::Blit(PSDLayer* image)
{
    Blit(image, m_preview);
}