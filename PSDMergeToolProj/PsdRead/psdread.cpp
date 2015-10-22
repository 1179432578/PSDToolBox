//
//  PSDExtractor.cpp
//  psdtool
//
//  Created by 徐海宁 on 15/2/16.
//  Copyright (c) 2015年 徐 海宁. All rights reserved.
//

#include "psdread.h"

void PSDLayer::SetName(psd_uchar* layerName)
{
    m_name = (char*)layerName;
}

PSDLayer* PSDLayer::NewAndAddChildLayer()
{
    PSDLayer* ret = new PSDLayer;
    m_children.push_back(ret);
    ret->m_parentLayer = this;
    return ret;
}

/// 只有在layer是normal时才进这个函数

PSDLayer* PSDLayer::NewAndAddChild(PSDLayer* parent, psd_layer_record* layerRecord)
{
    PSDLayer* child = parent->NewAndAddChildLayer();
    child->SetName(layerRecord->layer_name);
    child->m_left = layerRecord->left;
    child->m_top = layerRecord->top;
    child->m_right = layerRecord->right;
    child->m_bottom = layerRecord->bottom;
    child->m_width = layerRecord->width;
    child->m_height = layerRecord->height;
    child->m_pixels = layerRecord->image_data;
    child->m_visible = layerRecord->visible;
    //printf("layer name %s, layer id %ld\n", child->m_name.c_str(), layerRecord->layer_id);
    return child;
}


PSDLayer* PSDLayer::MakeLayerTree(psd_context* context)
{
    PSDLayer* root = new PSDLayer;
    root->SetName((psd_uchar*)"images");
    PSDLayer* currentLayer = root;

    // for test
//    psd_layer_type_normal = 0
//    psd_layer_type_hidden = 1
//    psd_layer_type_folder = 2
    for (int i=0; i<context->layer_count; i++) {
        psd_layer_record *layerInfo = &context->layer_records[i];
        printf("layerInfo: type:%d, name:%s, visible:%d\n",
               layerInfo->layer_type,
               layerInfo->layer_name,
               layerInfo->visible);
    }


    // transfer layer_records array to psdlayer tree
    for (psd_short i = 0; i < context->layer_count; i++) {
        psd_layer_record* layerRecord = &context->layer_records[i];

        if (layerRecord->layer_type == psd_layer_type_normal) {
            PSDLayer* child = NewAndAddChild(currentLayer, layerRecord);
        }
        else if (layerRecord->layer_type == psd_layer_type_hidden) {
            currentLayer = currentLayer->NewAndAddChildLayer();
        }
        else if (layerRecord->layer_type == psd_layer_type_folder) {
            currentLayer->SetName(layerRecord->layer_name);
            currentLayer = currentLayer->m_parentLayer;
        }
        else {
            /// skip
        }
    }

    return root;
}
