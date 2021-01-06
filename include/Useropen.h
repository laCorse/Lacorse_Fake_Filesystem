//
// Created by qianchen on 2020/11/17.
//

#ifndef LACORSE_FAKE_FILESYS_USEROPEN_H
#define LACORSE_FAKE_FILESYS_USEROPEN_H

#include <iostream>
#include <memory>
#include "Fcb.h"
#include <vector>
#include "Filesystem.h"
#include "Fat.h"

using namespace std;

class Useropen
{
public:///ke neng shi hu xiang bao han de wen ti
    Useropen(vector<string> &path, Fcb &fcb, Filesystem *fs) {
        dir = path;
        userFcb = fcb;
        pFs = fs;
        int first = fcb.first;
        while (pFs->*pfat1[first] != END) {
            file.push_back(new File(pFs->blocks[first]))
            first = pFs->*pfat1[first];
        }
        file.push_back(new File(pFs->blocks[first]));
        pos = 0;
    }

    Useropen(const Fcb &userFcb);

    ~Useropen() {
        for (int i = 0; i < file.size(); ++i) {
            delete file[i];
        }
    }

    /**
     * @brief use dir to check if it opened
     * @param otherOpen
     * @return
     */
    bool operator==(Useropen &otherOpen);

private:
    Fcb userFcb;            //用户打开的那个文件
    vector<File *> file;    //file content for 1 byte
    vector<string> dir;     //打开文件所在路径，以便检查文件是否已经打开
    int pos;                //读写指针的位置
    Filesystem *pFs;
};



class UseropenList {
public:
    Useropen *openList[10];
    int index;
};

#endif //LACORSE_FAKE_FILESYS_USEROPEN_H
