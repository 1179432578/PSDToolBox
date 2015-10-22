//
//  SimplePSD.cpp
//  psdrw
//
//  Created by 鲁飞 on 15/9/21.
//  Copyright (c) 2015年 鲁飞. All rights reserved.
//

#include "SimplePSD.h"
#include "float_base.h"
#include <iostream>
#include "dialog.h"
#include <QString>
#include <QDebug>

SimplePSD::SimplePSD(){
    m_psdWrite = new SFPSDWriter(EFloat2(768, 1024));
}

void SimplePSD::createForest(std::vector<std::string> &psdFileNames){
    for (int i=0; i!=psdFileNames.size(); i++) {
        psd_context * context = NULL;
        psd_status status;
        
        // parse the psd file to psd_context
        status = psd_image_load(&context, (char*)psdFileNames[i].c_str());
        
        // read psd
        m_forest.push_back(PSDLayer::MakeLayerTree(context));
        
        // free if it's done
        //psd_image_free(context);
    }
}

void markAllNode(PSDLayer *node){
    if (node) {
        node->m_bNeed = true;
        //std::cout<<"name:"<<node->m_name<<" is need"<<std::endl;
        for (int i=0; i!=node->m_children.size(); i++) {
            markAllNode(node->m_children[i]);
        }
    }
}

void visitTree(PSDLayer* root, std::vector<std::string> &path, int n, int num){
    if (QString::fromLocal8Bit(root->m_name.c_str()).toStdString() == path[n]) {
        root->m_bNeed = true;
        if (n + 1 == num) {
            // last subpath, set all psdlayer under the subpath bneed = tree
            markAllNode(root);
            return;
        }
    }
    for (int i=0; i!=root->m_children.size(); i++) {
        if (n + 1 < num && QString::fromLocal8Bit(root->m_children[i]->m_name.c_str()).toStdString() == path[n + 1]) {
            visitTree(root->m_children[i], path, n+1, num);
        }
    }
}

void SimplePSD::markForest(std::string &PsdLayersPath, int psdID){
    //parse path
    std::vector<std::string> path;
    const char * split = "/";
    path.push_back(strtok((char*)PsdLayersPath.c_str(),split));
    char *subPath;
    while(subPath = strtok(NULL,split)) {
        path.push_back(subPath);
    }
    
    // make if psdlayer is need then set psdlayer.bneed = true
    visitTree(m_forest[psdID], path, 0, path.size());
}

int totalLayerCount = 0;
int currentFinishLayerCount = 0;

void addToTree(PSDLayer *treeRoot, PSDLayer *parentPsdLayer){
    for (int i=0; i!=treeRoot->m_children.size(); i++) {
        if (treeRoot->m_children[i]->m_bNeed && treeRoot->m_children[i]->m_addCount == 0) {
            totalLayerCount++;
            treeRoot->m_children[i]->m_addCount++;
            PSDLayer *psdlayer = new PSDLayer;
            *psdlayer = *(treeRoot->m_children[i]);
            psdlayer->m_children.clear();
            parentPsdLayer->m_children.push_back(psdlayer);
            //std::cout<<"add to new tree, name:"<<psdlayer->m_name<<std::endl;
            addToTree(treeRoot->m_children[i], psdlayer);
        }
    }
}

void mergeTreeNode(PSDLayer *parent){
    if (!(parent->m_pixels)) {
        for (std::vector<PSDLayer *>::iterator i=parent->m_children.begin(); i!=parent->m_children.end(); i++) {
            for (std::vector<PSDLayer *>::iterator j = i+1; j!=parent->m_children.end();) {
                 //std::cout<<(*i)->m_name<<" "<<(*j)->m_name<<std::endl;
                if ((*i)->m_name == (*j)->m_name && !(*i)->m_pixels && !(*j)->m_pixels) {
                    totalLayerCount--;
                    // add j children nodes to i
                    for (int n=0; n!=(*j)->m_children.size(); n++) {
                        (*i)->m_children.push_back((*j)->m_children[n]);
                         //std::cout<<"add j to i children:"<<(*j)->m_name<<std::endl;
                    }
                    // delete j node
                    //std::cout<<"delete:"<<(*j)->m_name<<std::endl;
                    j = parent->m_children.erase(j);
                    // need delete *j release memory

                    mergeTreeNode(*i);
                }
                else{
                    j++;
                }
            }
        }
    }
}

bool isNumber(std::string &name){
    for (int i=0; i!=name.size(); i++) {
        if (name[i]>'9' || name[i]<'0') {
            return false;
        }
    }
    return true;
}

bool isNeedSort(std::vector<PSDLayer*> &sortObjects){
    for (int i=0; i!=sortObjects.size(); i++) {
        std::string name;
        name.push_back(i+1+'0');
        if (sortObjects[i]->m_name != name) {
            return true;
        }
    }
    return false;
}

std::string convertToString(int num){
    std::string str;
    while (num/10) {
        str.push_back(num%10+'0');
        num = num/10;
    }
    str.push_back(num +'0');
    std::reverse(str.begin(), str.end());
    return str;
}

void sortPixelLayer(PSDLayer *layer){
    // folder layer
    if (!layer->m_pixels) {
        std::vector<PSDLayer*> sortObjects;
        for (int i=0; i!=layer->m_children.size(); i++) {
            // pixel layer need be sorted
            if (layer->m_children[i]->m_pixels && isNumber(layer->m_children[i]->m_name)) {
                sortObjects.push_back(layer->m_children[i]);
            }
            else if(!layer->m_children[i]->m_pixels){
                sortPixelLayer(layer->m_children[i]);
            }
        }
        // check if need sort
        if (isNeedSort(sortObjects)) {
            for (int i=0; i!=sortObjects.size(); i++) {
                sortObjects[i]->m_name = convertToString(sortObjects.size()-i);
                //std::cout<<"new name:"<<sortObjects[i]->m_name<<std::endl;
            }
        }
    }
}

void SimplePSD::createNewTree(){
    m_newTree = new PSDLayer;
    *m_newTree = *(m_forest[0]);
    m_newTree->m_children.clear();
    for (int i=0; i!=m_forest.size(); i++) {
        addToTree(m_forest[i], m_newTree);
    }
    
    // merge tree's non-pixel layer which has similar name
    //std::cout<<"merge tree start"<<std::endl;
    mergeTreeNode(m_newTree);
    //std::cout<<"merge tree end"<<std::endl;
    
    sortPixelLayer(m_newTree);
}

void SimplePSD::writeNewTreeA(PSDLayer * layer){
    std::vector<PSDLayer*> &children = layer->m_children;
    std::wstring wstrName = QString::fromLocal8Bit(layer->m_name.c_str()).toStdWString();
    if (children.size() == 0) {
        // write pixel layer
        if (layer->m_pixels) {
            currentFinishLayerCount++;
            dialog->setProgressBar(currentFinishLayerCount, totalLayerCount);
            for (int y=0; y<layer->GetHeight(); y++) {
                for (int x=0; x<layer->GetWidth(); x++) {
                    psd_argb_color *pixel = &layer->m_pixels[y*layer->GetWidth()+x];
                    unsigned char *p = (unsigned char *)pixel;
                    unsigned char r = p[0];
                    unsigned char g = p[1];
                    unsigned char b = p[2];
                    p[2] = r;
                    p[0] = b;
                }
            }
            m_psdWrite->addLayerWithPSDLayer(layer, layer->m_name,
                                             wstrName, 1.0f,
                                             EFloat2(layer->m_left, layer->m_top));
        }
        return;
    }
    else{
        currentFinishLayerCount++;
        dialog->setProgressBar(currentFinishLayerCount, totalLayerCount);
        m_psdWrite->openGroupLayerWithName(layer->m_name,
                                           wstrName, 1.0f,
                                           true);
        for (int i=0; i!=children.size(); i++) {
            writeNewTreeA(children[i]);
        }
        m_psdWrite->closeCurrentGroupLayer();
    }
}

void restoreLayerColor(PSDLayer *layer){
    std::vector<PSDLayer*> &children = layer->m_children;
    if (children.size() == 0 && layer->m_bNeed) {
        if (layer->m_pixels) {
            for (int y=0; y<layer->GetHeight(); y++) {
                for (int x=0; x<layer->GetWidth(); x++) {
                    psd_argb_color *pixel = &layer->m_pixels[y*layer->GetWidth()+x];
                    unsigned char *p = (unsigned char *)pixel;
                    unsigned char r = p[0];
                    unsigned char g = p[1];
                    unsigned char b = p[2];
                    p[2] = r;
                    p[0] = b;
                }
            }
        }
        return;
    }
    else if(layer->m_bNeed){
        for (int i=0; i!=children.size(); i++) {
            restoreLayerColor(children[i]);
        }
    }
}


void SimplePSD::writePsdByNewTree(std::string savefilePath){
    //remove images node
    for(int i=0; i!=m_newTree->m_children.size(); i++){
        writeNewTreeA(m_newTree->m_children[i]);
    }

    MutableData* psdData = m_psdWrite->createPSDData2();
    psdData->WriteToFile( savefilePath.c_str());

    // restore color
    for(int i=0; i!= m_forest.size(); i++){
        restoreLayerColor(m_forest[i]);
    }

    delete psdData;

}

PSDLayer* findLayer(PSDLayer* root, std::vector<std::string> &path, int n, int num){
    if (QString::fromLocal8Bit(root->m_name.c_str()).toStdString() == path[n]) {
        if (n + 1 == num) {
            return root;
        }
    }
    for (int i=0; i!=root->m_children.size(); i++) {
//        qDebug("name: %s", QString::fromLocal8Bit(root->m_children[i]->m_name.c_str()).toStdString().c_str());
//        qDebug("name: %s %s", root->m_children[i]->m_name.c_str(), path[n+1].c_str());
        if (n + 1 < num && QString::fromLocal8Bit(root->m_children[i]->m_name.c_str()).toStdString() == path[n + 1]) {
            return findLayer(root->m_children[i], path, n+1, num);
        }
    }
}

PSDLayer* SimplePSD::findPsdLayer(std::string PsdLayersPath, int psdID){
    //parse path
    std::vector<std::string> path;
    const char * split = "/";
    path.push_back(strtok((char*)PsdLayersPath.c_str(),split));
    char *subPath;
    while(subPath = strtok(NULL,split)) {
        path.push_back(subPath);
    }

    //find layer
    PSDLayer *layer =  findLayer(m_forest[psdID], path, 0, path.size());
    return layer;
}
