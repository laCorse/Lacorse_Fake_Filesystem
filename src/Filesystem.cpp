//
// Created by qianchen on 2020/11/19.
//

#include "../include/Filesystem.h"



//! 成员函数

Filesystem::Filesystem()
{
    cout << "[Init]File System is initializing..." << endl;
    if (Init())
    {
        cout << "[Init]Initialized succeed." << endl;
        initialized_ = true;
        cout << "[Format]Start to format..."<<endl;
    }
    else
    {
        initialized_ = false;

    }

    //初始化失败直接宕机
    if(!initialized_)
        abort();
}



bool Filesystem::Init()
{
    cout << "[Init]Allocate disk space..."<<endl;
    //分配虚拟磁盘
    if (!ReadFileSys())
    {
        ptrToContent = new unsigned char[SIZE];

        //设置每个块的指针
        cout << "[Init]Set block pointers..."<<endl;
        for(int i = 0;i<BLOCKNUM;i++)
            blocks[i] = ptrToContent + BLOCKSIZE * i;

        //当前打开文件清空
        cout << "[Init]Reset open files' states"<<endl;
        for(int i=0;i<MAXOPENFILE;i++)
            openFiles[i].setToopenfile(false);
        if(Format()) cout<<"[Format]Format succeed."<<endl;
        else cout<<"[Format]Format failed."<<endl;
    } else
    {
        //设置每个块的指针
        cout << "[Init]Set block pointers..."<<endl;
        for(int i = 0;i<BLOCKNUM;i++)
            blocks[i] = ptrToContent + BLOCKSIZE * i;

        //当前打开文件清空
        cout << "[Init]Reset open files' states"<<endl;
        for(int i=0;i<MAXOPENFILE;i++)
            openFiles[i].setToopenfile(false);
        if(FormatBuffer()) cout<<"[Format]Format succeed."<<endl;
        else cout<<"[Format]Format failed."<<endl;

    }

}

bool Filesystem::Format()
{
    //block0初始化(使用placement new把托管第一块)
    pblock0 = static_cast<BLOCK0*>(new(blocks[0]) BLOCK0(5, blocks[5]));

    //FAT初始化(托管第二块、第三块)，前五块文件状态初始化
    pfat1 = static_cast<Fat*>(new(blocks[1]) Fat());
    pfat2 = static_cast<Fat*>(new(blocks[2]) Fat());
    for(int i = 0;i<5;i++)
    {
        pfat1->setid(i,END);
        pfat2->setid(i,END);
    }
    for(int i = 5;i<BLOCKNUM;i++)
    {
        pfat1->setid(i,FREE);
        pfat2->setid(i,FREE);
    }

    //创建根目录
    proot = static_cast<Fcb*>(new(blocks[3]) Fcb(".","",0,0,0,0,0));
    plastdir = static_cast<Fcb*>(new(blocks[3]+sizeof(Fcb)) Fcb("..","",0,0,0,0,0));

    //创建当前用户打开环境
    currentData.currentFileId = 0;
    return true;
}

bool Filesystem::FormatBuffer()
{
    //block0初始化
    pblock0 = reinterpret_cast<BLOCK0*>(blocks[0]);

    //FAT初始化，前五块文件状态初始化
    pfat1 = reinterpret_cast<Fat*>(blocks[1]);
    pfat2 = reinterpret_cast<Fat*>(blocks[2]);

    //创建根目录
    proot = reinterpret_cast<Fcb*>(blocks[3]);
    plastdir = reinterpret_cast<Fcb*>(blocks[3]+sizeof(Fcb));

    //创建当前用户打开环境
    currentData.currentFileId = 0;

    return true;
}

bool Filesystem::ReadFileSys()
{
    ifstream inFile("./lacorse_Fs_bak.dat", ios::in | ios::binary);
    if (!inFile)
    {
        cout << "[Read]Cannot find File System backup!" << endl;
        return false;
    }
    inFile.read((char*)ptrToContent,SIZE);
    cout << "[Read]Read File System backup suceed!" << endl;
    inFile.close();
    return true;
}

void Filesystem::SaveFileSys()
{
    ofstream outFile("./lacorse_Fs_bak.dat", ios::out | ios::binary);
    outFile.write((char*)ptrToContent, SIZE);
    outFile.close();
    cout << "[Save]Save File System succeed!" << endl;
}

Filesystem::~Filesystem()
{
    SaveFileSys();

    delete []ptrToContent;
}


