//
// Created by qianchen on 2020/11/17.
//

#ifndef LACORSE_FAKE_FILESYS_BLOCK0_H
#define LACORSE_FAKE_FILESYS_BLOCK0_H

#include <iostream>
#include <string>

extern "C"
{
#include <cstring>
}

using namespace std;

/**
 * @class Block0引导块
 */


class BLOCK0
{

public:
    BLOCK0(){};
    BLOCK0(char *pos,char *dataBlock)
    {
        memcpy(&Bk0,pos,sizeof(Bk0));
        //blockPos = pos;
        //memcpy(Block0,blockPos,sizeof(Block0));
        //init(5,dataBlock);
    }
    ~BLOCK0()
    {
        memcpy(blockPos,&Bk0,sizeof(Bk0));

    }

    bool init(unsigned short mroot, char *mstartblock)
    {
        Bk0.root = mroot;
        Bk0.startblock = mstartblock;
        memcpy(Bk0.information,"10101010",sizeof("10101010"));
    }


public:
    struct Block0
    {
        //!存储描述信息，如磁盘大小、磁盘块数量
        char information[200];
        //!根目录文件的起始盘块号
        unsigned short root;
        //!虚拟磁盘上数据开始的位置
        char *startblock;
    }Bk0;


    //!block0
    char* blockPos;
};

#endif //LACORSE_FAKE_FILESYS_BLOCK0_H
