//
// Created by qianchen on 2020/11/16.
//

#ifndef LACORSE_FAKE_FILESYS_COMMON_H
#define LACORSE_FAKE_FILESYS_COMMON_H

#include "BLOCK0.h"
#include "Useropen.h"
#include "Fat.h"
#include "Filesystem.h"
#include "Fcb.h"

#define BLOCKSIZE 1024  //磁盘块大小
#define SIZE 1024000    //虚拟磁盘空间大小
#define ROOTBLOCKNUM 2  //根目录初始所占盘块总数
#define MAXOPENFILE 10  //最多同时打开文件个数




#endif //LACORSE_FAKE_FILESYS_COMMON_H
