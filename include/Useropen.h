//
// Created by qianchen on 2020/11/17.
//

#ifndef LACORSE_FAKE_FILESYS_USEROPEN_H
#define LACORSE_FAKE_FILESYS_USEROPEN_H

#include <memory>
#include "Fcb.h"
#include <vector>

using namespace std;


class Useropen
{
public:
    Fcb userFcb;            //用户打开的那个文件
    char dir[80];           //打开文件所在路径，以便检查文件是否已经打开
    int count;              //读写指针的位置
    bool fcbState;          //文件的FCB是否被修改，1则是修改了
    bool topenfile;         //打开表项是否为空，false表示空
};


class UseropenList
{
public:
    Useropen openList[10];
    int index;


};

#endif //LACORSE_FAKE_FILESYS_USEROPEN_H
