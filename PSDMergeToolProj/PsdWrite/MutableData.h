//
//  MutableData.h
//  psdtool
//
//  Created by xhnsworks on 5/6/15.
//  Copyright (c) 2015 徐 海宁. All rights reserved.
//

#ifndef __psdtool__MutableData__
#define __psdtool__MutableData__

#include "float_base.h"
#include <vector>
#include <cstddef>
#include <memory.h>
#include <stdio.h>
#include "PSDSmartPtr.h"
#include <fstream>

using namespace std;

struct DataRange
{
    long location;
    long length;
};

class MutableData : public PSDRefObject
{
public:
    std::vector<unsigned char> m_data;
public:
    void AppendBytes(unsigned char* bytes, int length);
    void AppendValue(long value, int length);
    void AppendData(MutableData* data);
    void AppendUTF8String(const char* value, int length);
    void AppendColor(EFloat4 color, int length);
    void PackedBitsForRange(DataRange range, int skip, MutableData * dataOut);
    void ResizeAsImageRGBA(int width, int height);
    inline size_t Length() {
//        return m_data.size();
//        int id = (int)this;
//        char fileName[50];
//        sprintf(fileName, "C:/Users/Administrator/Desktop/PSDMergeTool/temple/%d.tmp", id);
//        ifstream fin;
//        fin.open(fileName, ios::in | ios::binary);
//        if(fin.is_open()){
//            fin.seekg(0, ios::end);
//            int fileSize = fin.tellg();
//            fin.close();
//            return fileSize + m_data.size();
//        }
//        else {
//           return m_bytes;
//        }
        return m_bytes;
    }
    void WriteToFile(const char* path);

    // write large data
    void writeToTempleFile();
    MutableData():m_bytes(0),m_id(++count){};
    ~MutableData(){
        char fileName[100];
        sprintf(fileName, "C:/Users/Administrator/Desktop/PSDMergeTool/temple/%d.tmp", m_id);
        remove(fileName);
        printf("delete file:%s\n", fileName);
    };
    int m_bytes;
    int m_id;
    static int count;
};

//typedef MutableData* MutableDataPtr;
typedef PSDSmartPtr<MutableData> MutableDataPtr;

#endif /* defined(__psdtool__MutableData__) */
