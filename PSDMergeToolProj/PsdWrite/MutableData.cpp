//
//  MutableData.cpp
//  psdtool
//
//  Created by xhnsworks on 5/6/15.
//  Copyright (c) 2015 徐 海宁. All rights reserved.
//

#include "MutableData.h"
#include <vector>
#include <fstream>

// 10MB
#define MAX_DATA_SIZE 102400000

using namespace std;

#define MIN_RUN		3		/* minimum run length to encode */
#define MAX_RUN		127		/* maximum run length to encode */
#define MAX_COPY	128		/* maximum characters to copy */

/* maximum that can be read before copy block is written */
#define MAX_READ	(MAX_COPY + MIN_RUN - 1)

int MutableData::count = 0;

void MutableData::AppendBytes(unsigned char* bytes, int length)
{
    m_bytes += length;

    for (int i = 0; i < length; i++) {
        // check
        if(m_data.size() == MAX_DATA_SIZE){
            writeToTempleFile();
        }

        m_data.push_back(bytes[i]);
    }  
}

void MutableData::AppendValue(long value, int length)
{
    if (length > 8)
        length = 8;

    m_bytes += length;

    unsigned char bytes[8]; 
    double divider = 1;
    for (int i = 0; i < length; i++){
        bytes[length - i - 1] = (long)(value / divider) % 256;
        divider *= 256;
    }
    for (int i = 0; i < length; i++) {
        // check
        if(m_data.size() == MAX_DATA_SIZE){
            writeToTempleFile();
        }

        m_data.push_back(bytes[i]);
    }
}

void MutableData::AppendData(MutableData* data)
{
    m_bytes += data->Length();

    // check m_data
    if(m_data.size() == MAX_DATA_SIZE){
        writeToTempleFile();
    }
    ifstream fin;
    char fileName1[100];
    unsigned int id = (unsigned int)data->m_id;
    sprintf(fileName1, "C:/Users/Administrator/Desktop/PSDMergeTool/temple/%d.tmp", id);
    fin.open(fileName1, ios::in | ios::binary);
    if(fin.is_open()){
        fstream fout;
        unsigned int id = (unsigned int)this->m_id;
        char fileName[100];
        sprintf(fileName, "C:/Users/Administrator/Desktop/PSDMergeTool/temple/%d.tmp", id);

        fout.open(fileName, ios::out | ios::app | ios::binary);
        if(!fout.is_open()){
            printf("cannot open file.\n");
        }

        // write m_data memory data to disk
        char *buf = new char[MAX_DATA_SIZE];
        for(int i=0; i!=m_data.size(); i++){
            buf[i] = m_data[i];
        }
        fout.write(buf, m_data.size());
        printf("AppendData m_data filename:%s size:%d\n", fileName, m_data.size());

        // clear m_data
        m_data.clear();
        // read fin write to fout
        while(!fin.eof()){
            fin.read(buf, MAX_DATA_SIZE);
            int readCount = fin.gcount();
            fout.write(buf, readCount);
            printf("AppendData read filename:%s  writeto filename:%s size:%d\n", fileName1, fileName, readCount);
        }
        delete buf;
        fout.close();
        fin.close();
    }

    for (int i = 0; i < data->m_data.size(); i++) {
        // check
        if(m_data.size() == MAX_DATA_SIZE){
            //printf("write m_data\n");
            writeToTempleFile();
        }

        m_data.push_back(data->m_data[i]);
    }
    //std::vector<unsigned char>(data->m_data).swap(data->m_data);
}

void MutableData::AppendUTF8String(const char* value, int length)
{
    m_bytes += length;

    for (int i = 0; i < length ; i++) {
        // check
        if(m_data.size() == MAX_DATA_SIZE){
            writeToTempleFile();
        }
        m_data.push_back(value[i]);
    }
}

void MutableData::AppendColor(EFloat4 color, int length)
{
    int redComponent = color.x * 65536.0 - 0.5;
    int greenComponent = color.y * 65536.0 - 0.5;
    int blueComponent = color.z * 65536.0 - 0.5;

    AppendValue(0, 2);
    AppendValue(redComponent, 2);
    AppendValue(greenComponent, 2);
    AppendValue(blueComponent, 2);
    AppendValue(0, 2);
}
typedef unsigned char                   UInt8;
#define YES true
#define NO false
void MutableData::PackedBitsForRange(DataRange range, int skip, MutableData * dataOut)
{
    const char * bytesIn = (const char*)&m_data[0];
    unsigned long bytesLength = range.location + range.length;
    unsigned long bytesOffset = range.location;
    //MutableData * dataOut = new MutableData;
    
    bool currIsEOF = NO;
    unsigned char currChar = 0;			    /* current character */
    unsigned char charBuf[MAX_READ];	/* buffer of already read characters */
    int count;						    /* number of characters in a run */
    memset(charBuf, 0, sizeof(charBuf));
    
    /* prime the read loop */
    currChar = bytesIn[bytesOffset];
    bytesOffset = bytesOffset + skip;
    count = 0;
    
    /* read input until there's nothing left */
    while (!currIsEOF)
    {
        charBuf[count] = (unsigned char)currChar;
        count++;
        
        if (count >= MIN_RUN) {
            int i;
            /* check for run  charBuf[count - 1] .. charBuf[count - MIN_RUN]*/
            for (i = 2; i <= MIN_RUN; i++){
                if (currChar != charBuf[count - i]){
                    /* no run */
                    i = 0;
                    break;
                }
            }
            
            if (i != 0)
            {
                /* we have a run write out buffer before run*/
                int nextChar;
                
                if (count > MIN_RUN){
                    /* block size - 1 followed by contents */
                    UInt8 a = count - MIN_RUN - 1;

                    dataOut->AppendBytes(&a, sizeof(UInt8));

                    dataOut->AppendBytes(charBuf, sizeof(unsigned char) * (count - MIN_RUN));
                }
                
                /* determine run length (MIN_RUN so far) */
                count = MIN_RUN;
                while (true) {
                    if (bytesOffset < bytesLength){
                        nextChar = bytesIn[bytesOffset];
                        bytesOffset += skip;
                    } else {
                        currIsEOF = YES;
                        nextChar = EOF;
                    }
                    if (nextChar != currChar) break;
                    
                    count++;
                    if (count == MAX_RUN){
                        /* run is at max length */
                        break;
                    }
                }
                
                /* write out encoded run length and run symbol */
                UInt8 a = ((int)(1 - (int)(count)));
                dataOut->AppendBytes(&a, sizeof(UInt8));
                dataOut->AppendBytes(&currChar, sizeof(UInt8));
                
                if ((!currIsEOF) && (count != MAX_RUN)){
                    /* make run breaker start of next buffer */
                    charBuf[0] = nextChar;
                    count = 1;
                } else {
                    /* file or max run ends in a run */
                    count = 0;
                }
            }
        }
        
        if (count == MAX_READ)
        {
            int i;
            
            /* write out buffer */
            UInt8 a = MAX_COPY - 1;
            dataOut->AppendBytes(&a, sizeof(UInt8));
            dataOut->AppendBytes(&charBuf[0], sizeof(unsigned char) * MAX_COPY);
            
            /* start a new buffer */
            count = MAX_READ - MAX_COPY;
            
            /* copy excess to front of buffer */
            for (i = 0; i < count; i++)
                charBuf[i] = charBuf[MAX_COPY + i];
        }
        
        if (bytesOffset < bytesLength)
            currChar = bytesIn[bytesOffset];
        else
            currIsEOF = YES;
        bytesOffset += skip;
    }
    
    /* write out last buffer */
    if (0 != count)
    {
        if (count <= MAX_COPY) {
            /* write out entire copy buffer */
            UInt8 a = count - 1;
            dataOut->AppendBytes(&a, sizeof(UInt8));
            dataOut->AppendBytes(charBuf, sizeof(unsigned char) * count);
        }
        else
        {
            /* we read more than the maximum for a single copy buffer */
            UInt8 a = MAX_COPY - 1;
            dataOut->AppendBytes(&a, sizeof(UInt8));
            dataOut->AppendBytes(charBuf, sizeof(unsigned char) * MAX_COPY);
            
            /* write out remainder */
            count -= MAX_COPY;
            a = count - 1;
            dataOut->AppendBytes(&a, sizeof(UInt8));
            dataOut->AppendBytes(&charBuf[MAX_COPY], sizeof(unsigned char) * count);
        }
    }
    
    //return dataOut;
}
void MutableData::ResizeAsImageRGBA(int width, int height)
{
    m_data.resize(width * height * 4);
}
void MutableData::WriteToFile(const char* path)
{
//    FILE* file = fopen(path, "wb");
//    fwrite(&m_data[0], 1, m_data.size(), file);
//    fclose(file);
    ifstream fin;
    char infile[100];
    int id = (int)this->m_id;
    sprintf(infile, "C:/Users/Administrator/Desktop/PSDMergeTool/temple/%d.tmp", id);
    fin.open(infile, ios::in | ios::binary);

    ofstream fout;
    fout.open(path, ios::out | ios::binary);
    if(!fout.is_open()){
        printf("cannot open file");
    }

    int fileSize = 0;
    char *buf = new char[MAX_DATA_SIZE];
    if(fin.is_open()){
        while (!fin.eof()) {
           fin.read(buf, MAX_DATA_SIZE);
           int count = fin.gcount();
           fout.write(buf, count);
           fileSize += count;
        }
    }

    fileSize += m_data.size();
    printf("file size:%d", fileSize);
    for(int i=0; i!=m_data.size(); i++){
        buf[i] = m_data[i];
    }
    fout.write(buf, m_data.size());
    delete buf;

    fin.close();
    fout.close();
}

void MutableData::writeToTempleFile(){
    ofstream fout;
    unsigned int id = (unsigned int)this->m_id;
    char fileName[100];
    sprintf(fileName, "C:/Users/Administrator/Desktop/PSDMergeTool/temple/%d.tmp", id);

    fout.open(fileName, ios::out | ios::app | ios::binary);
    if(!fout.is_open()){
        printf("cannot open file.");
    }

    // 10MB
    char *buf = new char[MAX_DATA_SIZE];
    for(int i=0; i!=MAX_DATA_SIZE; i++){
        buf[i] = m_data[i];
    }
    fout.write(buf, MAX_DATA_SIZE);
    delete buf;

    fout.close();

    // clear m_data
    m_data.clear();
}
