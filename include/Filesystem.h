//
// Created by qianchen on 2020/11/19.
//

#ifndef LACORSE_FAKE_FILESYS_FILESYSTEM_H
#define LACORSE_FAKE_FILESYS_FILESYSTEM_H

#include <iostream>
#include <memory>
#include <fstream>
#include "Useropen.h"
#include "BLOCK0.h"
#include "Fat.h"
#include "Commandline.h"

using namespace std;

#define __version__ "1.0"

struct currentData
{
    //!当前目录的文件块号
    int currentFileId;
    //!记录当前目录的目录名（包括目录的路径）,想用string替代
    char currentdir[80];
};

/**
 * @class 核心文件系统
 */

class Filesystem
{

public:
    /**
     * @brief:Filesystem ctor function
     */
    Filesystem();

    /**
     * @brief Initialization.
     * @note 主要是分配空间和各块指针位置
     * @return True if initialize success
     */
    bool Init();

    /**
     * @brief Format.
     * @note 主要是对引导块、文件分配表、根目录区数据结构性内容进行初始化，主要采用了placement new在新申请的内存上直接分配
     * @return True if format success
     */
    bool Format();

    /**
     * @brief Format.
     * @note 对读入的内存进行初始化,与Format()不同点在于初始化的是各类的指针，也就是没有新分配内存（无关类型强转）
     * @return True if format success
     */
    bool FormatBuffer();

    /**
     * @brief dtor
     */
    ~Filesystem();

    /**
     * @brief Read fs
     */
    bool ReadFileSys();

    /**
     * @brief Save fs
     */
    void SaveFileSys();

    /**
     * @brief Get currentdir
     * @return
     */
    const currentData GetCurrentData()const
    {
        return currentData;
    }

private:
    //!是否已经初始化
    bool initialized_;
    //!文件块的起始地址,分配SIZE(1024000字节)大小的空间给该指针(存储也利用该指针)。
    unsigned char * ptrToContent;
    //!分别指向各个块地址，第一块为引导块，2~3块为FAT1,4~5为FAT2，剩余995块为数据区。
    unsigned char * blocks[BLOCKNUM];
    //!打开的文件列表
    Useropen openFiles[MAXOPENFILE];
    //!引导块,托管blocks[0]
    BLOCK0 * pblock0;
    //!FAT1/2
    Fat * pfat1;
    Fat * pfat2;

    //!指向当前目录和上一目录的指针
    Fcb * proot;
    Fcb * plastdir;

    //!保存当前目录的信息
    currentData currentData;



};




#endif //LACORSE_FAKE_FILESYS_FILESYSTEM_H
