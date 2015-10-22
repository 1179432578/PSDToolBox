//
//  PSDExtractor.h
//  psdtool
//
//  Created by 徐海宁 on 15/2/16.
//  Copyright (c) 2015年 徐 海宁. All rights reserved.
//

#ifndef __psdtool__PSDExtractor__
#define __psdtool__PSDExtractor__

#include <stdio.h>
#include <vector>
#include <set>
#include <string>
#include "libpsd.h"

class PSDLayer
{
public:
    bool m_bNeed;
    int m_addCount;
    psd_bool m_visible;
    PSDLayer* m_parentLayer;
    std::vector<PSDLayer*> m_children;
    std::string m_name;
    psd_int m_left;
    psd_int m_top;
    psd_int m_right;
    psd_int m_bottom;
    psd_int m_exportLeft;
    psd_int m_exportTop;
    psd_int m_exportRight;
    psd_int m_exportBottom;
    psd_int m_width;
    psd_int m_height;
    // dir:null image:not null
    psd_argb_color* m_pixels;
    psd_int m_pngWidth;
    psd_int m_pngHeight;
    psd_argb_color* m_pngPixels;
private:
    void SetName(psd_uchar* layerName);
    PSDLayer* NewAndAddChildLayer();
private:
    static PSDLayer* NewAndAddChild(PSDLayer* parent, psd_layer_record* layerRecord);
public:
    PSDLayer()
    : m_parentLayer(nullptr)
    , m_addCount(0)
    , m_bNeed(false)
    , m_left(0)
    , m_top(0)
    , m_right(0)
    , m_bottom(0)
    , m_exportLeft(0)
    , m_exportTop(0)
    , m_exportRight(0)
    , m_exportBottom(0)
    , m_width(0)
    , m_height(0)
    , m_pixels(nullptr)
    , m_pngWidth(0)
    , m_pngHeight(0)
    , m_pngPixels(nullptr)
    {}
     inline PSDLayer* GetParent() {
        return m_parentLayer;
    }
    inline const std::string& GetName() const {
        return m_name;
    }
    inline psd_int GetLeft() const {
        return m_left;
    }
    inline psd_int GetRight() const {
        return m_right;
    }
    inline psd_int GetTop() const {
        return m_top;
    }
    inline psd_int GetBottom() const {
        return m_bottom;
    }
    inline psd_int GetExportLeft() const {
        return m_exportLeft;
    }
    inline psd_int GetExportRight() const {
        return m_exportRight;
    }
    inline psd_int GetExportTop() const {
        return m_exportTop;
    }
    inline psd_int GetExportBottom() const {
        return m_exportBottom;
    }
    inline psd_int GetWidth() const {
        return m_width;
    }
    inline psd_int GetHeight() const {
        return m_height;
    }
    inline psd_int GetExportWidth() const {
        return m_pngWidth;
    }
    inline psd_int GetExportHeight() const {
        return m_pngHeight;
    }
    inline psd_argb_color* GetPixels() {
        return m_pixels;
    }
    inline psd_argb_color* GetPNGPixels() {
        return m_pngPixels;
    }
public:
    static PSDLayer* MakeLayerTree(psd_context* context);
};

#endif /* defined(__psdtool__PSDExtractor__) */
