//
// Created by qianchen on 2020/11/17.
//

#ifndef LACORSE_FAKE_FILESYS_USEROPEN_H
#define LACORSE_FAKE_FILESYS_USEROPEN_H

#include <iostream>
#include <memory>
#include <cstring>
#include <vector>
#include "Fat.h"
#include "Fcb.h"

using namespace std;

enum {
    CUT = 0,
    COVER,
    ADD
};


/**
 * @class File: delegate file document
 */
class File
{
public:
    File(char * file):originPlace(file)
    {
        memcpy(content,file,sizeof(BLOCKSIZE));
    }


    ~File()
    {
        memcpy(originPlace,content,sizeof(BLOCKSIZE));
    }

    string read_content()
    {
        return string(content,BLOCKSIZE);
    }

    void change_content(char * text,int pos, int size)
    {
        memcpy(content+pos,text,size);
    }


private:
    char content[BLOCKSIZE];
    char * originPlace;


};


class Useropen
{

public:
    /**
     * @brief empty
     */
    Useropen(){}

    /**
     * @brief not empty
     * @param path
     * @param fcb
     * @param mfat
     * @param mblocks
     */
    Useropen(vector<string> &path, Fcb &fcb, Fat *mfat,char** mblocks) {
        dir = path;
        userFcb = fcb;
        pFat = mfat;
        int first = fcb.first;
        blocks = mblocks;
        while ((*pFat)[first] != END)
        {
            file.push_back(new File(blocks[first]));
            first = (*pFat)[first];
        }
        file.push_back(new File(blocks[first]));
        pos = 0;
    }

    ~Useropen() {
        for (int i = 0; i < file.size(); ++i) {
            delete file[i];
        }
    }

    /**
     * @brief like 'cat'
     * @return
     */
    bool read();

    /**
     * @brief write content
     * @return
     */
    bool write(int mode, char* text);

    /**
     * @brief use dir to check if it opened
     * @param otherOpen
     * @return
     */
    bool operator==(Useropen &otherOpen);

    bool empty()
    {
        if (dir.empty())
        {
            return true;
        }
        else
            return false;
    }

private:
    Fcb userFcb;            //用户打开的那个文件
    vector<File *> file;    //file content for 1 byte
    vector<string> dir;     //打开文件所在路径，以便检查文件是否已经打开
    int pos;                //读写指针的位置
    Fat *pFat;
    char** blocks;          //指针，指向文件块


    bool setMode(int mode);


    int lastBlockId();
};



class UseropenList {
public:
    Useropen *openList[10];
    int index;
};

#endif //LACORSE_FAKE_FILESYS_USEROPEN_H
