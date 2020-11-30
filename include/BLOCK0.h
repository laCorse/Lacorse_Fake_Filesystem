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
    BLOCK0(unsigned short root, unsigned char *startblock)
    :root(root),startblock(startblock),information{"10101010"}
    {

    }

    BLOCK0(const BLOCK0& block0)
    {
        *(this->startblock) = *(block0.startblock);
        this->root = block0.root;
        memcpy(this->information,block0.information,strlen(block0.information)+1);
    }
    BLOCK0 & operator=(const BLOCK0& block0)
    {
        *(this->startblock) = *(block0.startblock);
        this->root = block0.root;
        memcpy(this->information,block0.information,strlen(block0.information)+1);
        return *this;
    }
private:
    //!存储描述信息，如磁盘大小、磁盘块数量
    char information[200];
    //!根目录文件的起始盘块号
    unsigned short root;
    //!虚拟磁盘上数据开始的位置
    unsigned char *startblock;
};

#endif //LACORSE_FAKE_FILESYS_BLOCK0_H
