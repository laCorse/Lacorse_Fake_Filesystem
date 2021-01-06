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
#include <algorithm>
#include <cstring>
#include "common.h"
#include <string>
#include <regex>
#include <iterator>

#define __version__ "1.0"
using namespace std;
/**
 * @brief 对in利用delim进行划分
 * @param in 输入字符串，如文件路径
 * @param delim 分隔符
 * @return 存放除分隔符外的目录路径名
 */
vector<string> stringSplit(const string &in, const string &delim);

#define DEBUG

class Document {
public:
    Document() {};
    Document(char *block)
    {
        pos = block;
        constexpr int size = BLOCKSIZE/sizeof(Fcb);
        for(int i=0;i<size;i++)
        {
            char *tmp = pos+i*sizeof(Fcb);
            Fcb * tmpFcb = (Fcb *)tmp;
            //memcpy(&tmpFcb,tmp,sizeof(Fcb));
            if (tmpFcb->free == 1)
            {
                fcbList.push_back(*tmpFcb);
            }
        }

    };

    Document(vector<Fcb> fList) : fcbList(fList) {pos = nullptr;}

    Document(Document & doc)
    {
        doc.save();
        pos = doc.pos;

        constexpr int size = BLOCKSIZE/sizeof(Fcb);
        for(int i=0;i<size;i++)
        {
            char *tmp = pos+i*sizeof(Fcb);
            Fcb * tmpFcb = (Fcb *)tmp;
            //memcpy(&tmpFcb,tmp,sizeof(Fcb));
            if (tmpFcb->free == 1)
            {
                fcbList.push_back(*tmpFcb);
            }
        }
    }



    ~Document()
    {
        if (pos)
        {
            for(int i=0;i<fcbList.size();i++)
            {
                memcpy(pos+i*sizeof(Fcb),&fcbList[i],sizeof(Fcb));
            }

        }
    }

    void update()
    {
        fcbList.clear();
        constexpr int size = BLOCKSIZE/sizeof(Fcb);
        for(int i=0;i<size;i++)
        {
            char *tmp = pos+i*sizeof(Fcb);
            Fcb * tmpFcb = (Fcb *)tmp;
            //memcpy(&tmpFcb,tmp,sizeof(Fcb));
            if (tmpFcb->free == 1)
            {
                fcbList.push_back(*tmpFcb);
            }
        }
    }

    void save()
    {
        if (pos)
        {
            for(int i=0;i<fcbList.size();i++)
            {
                memcpy(pos+i*sizeof(Fcb),&fcbList[i],sizeof(Fcb));
            }

        }
    }

    void set_list(vector<Fcb> mList) {
        fcbList = mList;
    }

    vector<Fcb> fcbList;
    char *pos;
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


private:
    char content[BLOCKSIZE];
    char * originPlace;


};

/**
 * @class 核心文件系统
 */

class Filesystem {
    struct currentData {
        //!当前目录的文件块号
        int currentFileId;
        //!记录当前目录的目录名（包括目录的路径）,想用string替代
        vector<string> path;
        //!current document files' fcb
        Document currentDoc;
    };

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
    const currentData & GetCurrentData() const {
        return currentData;
    }

    /**
     * @brief read current document
     * @param index: block id of dir file
     */
    void read_dir(int index, string sPath) {
        currentData.currentDoc.fcbList.clear();
        currentData.path.clear();
        currentData.currentFileId=-1;

        Document *dirPtr = new Document(blocks[index]);
        currentData.currentDoc.fcbList = dirPtr->fcbList;

        //cout << "rsize: "<<currentData.currentDoc.fcbList.size()<<endl;

        auto paths = stringSplit(sPath, "/");
        if (!paths.empty())
        {
            auto it = paths.begin();
            if (*it == "")
                paths.erase(it);
        }
        currentData.path = paths;
        currentData.currentFileId = index;
        delete dirPtr;
    }

    void read_dir(int index, vector<string> sPath) {
        currentData.currentDoc.fcbList.clear();
        currentData.path.clear();
        currentData.currentFileId=-1;


        Document *dirPtr = new Document(blocks[index]);
        currentData.currentDoc.fcbList = dirPtr->fcbList;

        if (!sPath.empty())
        {
            auto it = sPath.begin();
            if (*it == "")
                sPath.erase(it);
        }

        currentData.path = sPath;

        currentData.currentFileId = index;
        delete dirPtr;
    }

    /**
     * @brief search a file(absolute path)
     */
    bool search_file(vector<string> paths,int & id);


    /**
     * @brief copy this file through id
     */
    shared_ptr<Document> copy_file(int id) {
        vector<int> idList;
        idList.push_back(id);
        while ((*pfat1)[id] != END && (*pfat1)[id] != FREE) {
            id = (*pfat1)[id];
            idList.push_back(id);
        }
        vector<Fcb> fcbList;
        for (int i = 0; i < idList.size(); i++) {
            Document *tmpDoc = new Document(blocks[idList[i]]);
            fcbList.insert(fcbList.end(), tmpDoc->fcbList.begin(), tmpDoc->fcbList.end());
            delete tmpDoc;
        }
        shared_ptr<Document> ptr = make_shared<Document>(fcbList);
        return ptr;

    }
    vector<Fcb> copy_file_vec(int id) {
        vector<int> idList;
        idList.push_back(id);
        while ((*pfat1)[id] != END && (*pfat1)[id] != FREE) {
            id = (*pfat1)[id];
            idList.push_back(id);
        }
        vector<Fcb> fcbList;
        for (int i = 0; i < idList.size(); i++) {
            Document *tmpDoc = new Document(blocks[idList[i]]);
            fcbList.insert(fcbList.end(), tmpDoc->fcbList.begin(), tmpDoc->fcbList.end());
            delete tmpDoc;
        }
        return fcbList;

    }

    int copy_file_lastid(int id) {
        vector<int> idList;
        idList.push_back(id);
        while ((*pfat1)[id] != END && (*pfat1)[id] != FREE) {
            id = (*pfat1)[id];
            idList.push_back(id);
        }
        return idList.back();

    }

    /**
     * @brief ls
     * @param paths
     */
    bool list_file(vector<string> paths);

    /**
     * @brief mkdir
     * @param paths
     * @return
     */
    bool make_dir(vector<string> paths);

    /**
     * @brief add a fcb in document file
     */
    bool add_fcb(int firstId,Fcb fcb);

    /**
     * @brief make a file
     * @param paths
     * @return
     */
    bool make_file(vector<string> paths);

private:
    //!是否已经初始化
    bool initialized_;
    //!文件块的起始地址,分配SIZE(1024000字节)大小的空间给该指针(存储也利用该指针)。
    char *ptrToContent;
    //!分别指向各个块地址，第一块为引导块，2~3块为FAT1,4~5为FAT2，剩余995块为数据区。
    char *blocks[BLOCKNUM];
    //!打开的文件列表
    Useropen openFiles[MAXOPENFILE];
    //!引导块,托管blocks[0]
    BLOCK0 *pblock0;
    //!FAT1/2
    Fat *pfat1;
    Fat *pfat2;

    //!root dir
    Document *proot;
    Document *plastdir;


    //!保存当前目录的信息
    currentData currentData;


};


#endif //LACORSE_FAKE_FILESYS_FILESYSTEM_H
