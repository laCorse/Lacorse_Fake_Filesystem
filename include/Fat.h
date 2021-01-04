//
// Created by qianchen on 2020/11/17.
//

#ifndef LACORSE_FAKE_FILESYS_FAT_H
#define LACORSE_FAKE_FILESYS_FAT_H

#define END 65535       //FAT中文件的结束标志
#define FREE 0          //FAT中盘块空闲标志
#define FULL -1

#define BLOCKSIZE 1024  //磁盘块大小
#define SIZE 1024000    //虚拟磁盘空间大小
#define MAXOPENFILE 10  //最多同时打开文件个数

constexpr int BLOCKNUM = SIZE / BLOCKSIZE;

/**
 * @class Fat表
 */

class Fat
{
        typedef unsigned short Filelist;

public:
    Fat(char* pos):postion(pos)
    {
        memcpy(filelist,postion,sizeof(filelist));
        init_filelist();
    }

    ~Fat()
    {
        if (postion)
        {
            memcpy(postion,filelist,sizeof(filelist));
        }
    }


    Filelist filelist[BLOCKNUM];
    char * postion;


    bool init_filelist()
    {
        for(int i = 0;i<6;i++)
        {
            filelist[i] = END;
        }
        for(int i = 6;i<BLOCKNUM;i++)
        {
            filelist[i] = FREE;
        }
//        for (int i = 0; i < BLOCKNUM; ++i) {
//            cout << filelist[i]<<endl;
//        }

    }

    /**
     * @brief 重载了下[]，便于直接访问（不安全）
     * @param id
     * @return
     */
    unsigned short& operator[](int id)
    {
        return filelist[id];
    }

//    /**
//     * @brief 对number个文件状态设置状态status
//     * @param number
//     * @param status
//     */
//    void setid(int number, int status)
//    {
//        filelist[number].id_ = status;
//    }

    int get_FreeBlock()
    {

        for (int i = 6; i < BLOCKNUM; ++i) {
            if (filelist[i] == FREE)
                return i;
        }
        return FULL;
    }

};

#endif //LACORSE_FAKE_FILESYS_FAT_H
