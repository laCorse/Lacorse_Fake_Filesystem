//
// Created by qianchen on 2020/11/17.
//

#ifndef LACORSE_FAKE_FILESYS_FCB_H
#define LACORSE_FAKE_FILESYS_FCB_H

#include <iostream>

extern "C"
{
#include <cstring>
}


using namespace std;

//class Fcb
//{
//public:
//    /**
//     * @brief ctors
//     */
//    Fcb(){};
//    Fcb(const char mfilename[], const char mexname[],
//            unsigned char attribute,unsigned short time,
//            unsigned short date,int first,
//            unsigned long length,char free = 0)
//    {
//        if (!init(mfilename, mexname,attribute,time,date, first,length,free))
//        {
//            cout << "[FCB]Creating failed"<<endl;
//        }
//    }
//    /**
//     * @brief Initialization.
//     * @note
//     * @return True if initialize success
//     */
//    bool init(const char mfilename[], const char mexname[],
//              unsigned char & mattribute,unsigned short & mtime,
//              unsigned short & mdate,int & mfirst,
//              unsigned long & mlength,char & mfree)
//    {
//        if((strlen(mfilename)<8)&&(strlen(mexname)<3))
//        {
//            memcpy(filename,mfilename,strlen(mfilename)+1);
//            memcpy(exname,mexname,strlen(mfilename)+1);
//            attribute = mattribute;
//            time = mtime;
//            date = mdate;
//            first = mfirst;
//            length = attribute?mlength:0;
//            free = mfree;
//
//            return true;
//        }
//        else
//        {
//            cout << "[FCB]The filename of exname is too long!" << endl;
//            return false;
//        }
//
//    }
//
//
//public:
//    //!文件名
//    char filename[8];
//    //!文件拓展名
//    char exname[3];
//    //!文件属性字段，0表示目录，1表示数据文件
//    unsigned char attribute;
//    //!文件创建时间
//    unsigned short time;
//    //!文件创建日期
//    unsigned short date;
//    //!文件起始盘块号
//    int first;
//    //!文件长度（字节数）
//    unsigned long length;
//    //!表示目录项是否为空，若值为0表示空，1表示已分配
//    char free;
//};

struct Fcb
{
public:
    //!文件名
    char filename[8];
    //!文件拓展名
    char exname[3];
    //!文件属性字段，0表示目录，1表示数据文件
    unsigned char attribute;
    //!文件创建时间
    unsigned short time;
    //!文件创建日期
    unsigned short date;
    //!文件起始盘块号
    int first;
    //!文件长度（字节数）
    unsigned long length;
    //!表示目录项是否为空，若值为0表示空，1表示已分配
    char free;
};

bool initFcb(Fcb & fcb,const char mfilename[], const char mexname[],
          unsigned char mattribute,unsigned short mtime,
          unsigned short mdate,int mfirst,
          unsigned long mlength,char mfree);

#endif //LACORSE_FAKE_FILESYS_FCB_H
