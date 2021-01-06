//
// Created by qianchen on 2020/11/19.
//

#include "../include/Filesystem.h"

#define DEBUG

vector<string> stringSplit(const string &in, const string &delim) {
    regex re{delim};
    return vector<std::string>{
            sregex_token_iterator(in.begin(), in.end(), re, -1),
            sregex_token_iterator()
    };
}


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
        ptrToContent = new char[SIZE]{0};

        //设置每个块的指针
        cout << "[Init]Set block pointers..."<<endl;
        for(int i = 0;i<BLOCKNUM;i++)
            blocks[i] = ptrToContent + BLOCKSIZE * i;

        //当前打开文件清空
        cout << "[Init]Reset open files' states"<<endl;
        for(int i=0;i<MAXOPENFILE;i++)
            openFiles[i].topenfile = false;
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
            openFiles[i].topenfile = false;
        if(FormatBuffer()) cout<<"[Format]FormatBuffer succeed."<<endl;
        else cout<<"[Format]FormatBuffer failed."<<endl;

    }
    return true;

}

bool Filesystem::Format()
{
    //block0初始化(使用placement new把托管第一块)
    pblock0 = new BLOCK0(blocks[0],blocks[5]);

    //FAT初始化(托管第二块、第三块)，前五块文件状态初始化
    pfat1 = new Fat(blocks[1]);
    pfat2 = new Fat(blocks[3]);

    //创建根目录
    proot = new Document(blocks[5]);
    read_dir(5,string());

    return true;
}

bool Filesystem::FormatBuffer()
{
    //block0初始化
    pblock0 = new BLOCK0(blocks[0],blocks[5]);

    //FAT初始化，前五块文件状态初始化
//    pfat1 = reinterpret_cast<Fat*>(blocks[1]);
//    pfat2 = reinterpret_cast<Fat*>(blocks[2]);
    pfat1 = new Fat(blocks[1]);
    pfat2 = new Fat(blocks[3]);


    //创建根目录
    proot = new Document(blocks[5]);

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
    cout << "[Read]Read File System backup succeed!" << endl;
    inFile.close();
    return false;
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


    delete pblock0;
    delete pfat1;
    delete pfat2;
    delete proot;
    delete []ptrToContent;
}


bool Filesystem::search_file(vector<string> paths,int & id)
{
    if (!paths.empty())
    {
        auto it = paths.begin();
        if (*it == "")
            paths.erase(it);
    }
    proot->update();
#ifdef DEBUG
    cout <<"root "<< proot->fcbList.size() << endl;
#endif
    Document * curDir = new Document(*proot);
    Fcb curFcb;
    bool findFlag = false;
    int blockId;

#ifdef DEBUG
    cout <<"path "<< paths[0] << endl;
#endif
    if (paths.size()==0 || paths.empty())
    {
        id=5;
    }
    else
    {
        for(int j=0;j<paths.size();j++)
        {
#ifdef DEBUG
            cout << curDir->fcbList[0].filename << endl;
            cout << strcmp(curDir->fcbList[0].filename,paths[0].c_str())<<endl;
#endif
            // search curDir
            for (int i = 0; i < curDir->fcbList.size(); ++i)
            {

                if (strcmp(curDir->fcbList[i].filename,paths[j].c_str()) ==0)
                {
                    if (curDir->fcbList[i].attribute==0)
                    {
                        findFlag = true;
                        curFcb = curDir->fcbList[i];
                        blockId = curFcb.first;
                        id = curFcb.first;
                        break;
                    }

                }
            }
            if (!findFlag)
            {
                return false;
            }
            else
            {
                delete curDir;
                auto vec = copy_file_vec(blockId);
                curDir = new Document(vec);
                findFlag = false;
            }
        }

    }

#ifdef DEBUG
    cout <<"id "<< id << endl;
#endif
    return true;
}


bool Filesystem::list_file(vector<string> paths)
{
    if (!paths.empty())
    {
        auto it = paths.begin();
        if (*it == "")
            paths.erase(it);
    }
#ifdef DEBUG
    cout <<"root "<< proot->fcbList.size() << endl;
#endif
    Document * curDir =new Document(*proot);
    Fcb curFcb;
    bool findFlag = false;
    int blockId;
    for(int j=0;j<paths.size();j++)
    {
        // search curDir
        for (int i = 0; i < curDir->fcbList.size(); ++i)
        {
            if (strcmp(curDir->fcbList[i].filename,paths[j].c_str()) ==0)
            {
                if (curDir->fcbList[i].attribute==0)
                {
                    findFlag = true;
                    curFcb = curDir->fcbList[i];
                    blockId = curFcb.first;
                    break;
                }

            }
        }
        if (!findFlag)
        {
            return false;
        }
        else
        {
            delete curDir;
            auto vec = copy_file_vec(blockId);
#ifdef DEBUG

#endif
            curDir = new Document(vec);
            findFlag = false;
        }
    }


    if (curDir->pos!=proot->pos)
        delete curDir;

    for (auto & fcb:curDir->fcbList)
    {
        cout<<fcb.filename<<fcb.exname<<"\t";
    }
    return true;
}

bool Filesystem::make_dir(vector<string> paths)
{

    if (!paths.empty())
    {
        auto it = paths.begin();
        if (*it == "")
            paths.erase(it);
    }
#ifdef DEBUG
    cout <<"root "<< proot->fcbList.size() << endl;
#endif

    Document *curDir = new Document(*proot);
    Fcb curFcb;
    bool findFlag = false;
    int blockId = 5;
#ifdef DEBUG
    cout<< "newID: " << blockId <<endl;
#endif
//    if (paths.size() == 1)
//        blockId = 5;


    string needtoMake = paths.back();
#ifdef DEBUG
    cout<< "needtoMake: " << needtoMake <<endl;
#endif
    for(int j=0;j<paths.size()-1;j++)
    {
        // search curDir
        for (int i = 0; i < curDir->fcbList.size(); ++i)
        {
            if (strcmp(curDir->fcbList[i].filename,paths[j].c_str()) ==0)
            {
                if (curDir->fcbList[i].attribute==0)
                {
                    findFlag = true;
                    curFcb = curDir->fcbList[i];
                    blockId = curFcb.first;
                    break;
                }

            }
        }
        if (!findFlag)
        {
            return false;
        }
        else
        {
            if (curDir->pos != proot->pos)
                delete curDir;
            auto vec = copy_file_vec(blockId);
#ifdef DEBUG

#endif
            curDir = new Document(vec);
            findFlag = false;
        }
    }

    curDir->save();
    if (curDir->pos!=proot->pos)
        delete curDir;
#ifdef DEBUG
    cout <<"root "<< proot->fcbList.size() << endl;
#endif

    /// create the file in curDir
    Fcb newFcb;
    //allocate new block
    int newBlock = pfat1->get_FreeBlock();
    (*pfat1)[newBlock] = END;
    initFcb(newFcb,needtoMake.c_str(),"",0,0,0,newBlock,0,1);
    if(!add_fcb(blockId,newFcb))
    {
        cout << "[Err]Cannot make this fold!"<<endl;
        return false;
    }


#ifdef DEBUG
//    cout <<"list_file"<<endl;
//    list_file(vector<string>());
//    cout <<endl;
#endif
    return true;

}

///2 STEPs: 1.get the last id 2. insert fcb and change the fat
bool Filesystem::add_fcb(int firstId,Fcb fcb)
{
    ///Step1
    int lastId = firstId;
    if ((*pfat1)[lastId]== FREE)
    {
        (*pfat1)[lastId]= END;
    }
    while ((*pfat1)[lastId] != END) {
        lastId = (*pfat1)[lastId];
    }
    Document* plastDoc = new Document(blocks[lastId]);
#ifdef DEBUG
    cout<< "last id: " <<lastId<<endl;
    cout<< "plastDoc->fcbList: " <<plastDoc->fcbList.size()<<endl;
#endif
    if((plastDoc->fcbList.size()+1)*sizeof(Fcb)>= BLOCKSIZE)
    {
        int freeBlock = pfat1->get_FreeBlock();
        if (freeBlock==FULL)
        {
            cout << "[Err]Current content is full!"<<endl;
            return false;
        }
        (*pfat1)[lastId] = freeBlock;
        (*pfat1)[freeBlock] = END;
        delete plastDoc;
        plastDoc = new Document(blocks[lastId]);
    ///Step2
        plastDoc->fcbList.push_back(fcb);
    }
    else
    {
        plastDoc->fcbList.push_back(fcb);
    }
#ifdef DEBUG
    //cout << "size:" << plastDoc->fcbList.size()<<endl;
    //cout << plastDoc->fcbList[0].filename<<endl;
#endif
    plastDoc->save();
    delete plastDoc;
    return true;

}

bool Filesystem::make_file(vector<string> paths)
{

    if (!paths.empty())
    {
        auto it = paths.begin();
        if (*it == "")
            paths.erase(it);
    }

    Document *curDir = new Document(*proot);
    Fcb curFcb;
    bool findFlag = false;
    int blockId = 5;

    string needtoMake = paths.back();
    for(int j=0;j<paths.size()-1;j++)
    {
        // search curDir
        for (int i = 0; i < curDir->fcbList.size(); ++i)
        {
            if (strcmp(curDir->fcbList[i].filename,paths[j].c_str()) ==0)
            {
                if (curDir->fcbList[i].attribute==0)
                {
                    findFlag = true;
                    curFcb = curDir->fcbList[i];
                    blockId = curFcb.first;
                    break;
                }

            }
        }
        if (!findFlag)
        {
            return false;
        }
        else
        {
            if (curDir->pos != proot->pos)
                delete curDir;
            auto vec = copy_file_vec(blockId);
            curDir = new Document(vec);
            findFlag = false;
        }
    }

    curDir->save();
    if (curDir->pos!=proot->pos)
        delete curDir;

    Fcb newFcb;
    //allocate new block
    int newBlock = pfat1->get_FreeBlock();
    (*pfat1)[newBlock] = END;
    initFcb(newFcb,needtoMake.c_str(),"",1,0,0,newBlock,0,1);
    if(!add_fcb(blockId,newFcb))
    {
        cout << "[Err]Cannot make this file!"<<endl;
        return false;
    }

    return true;

}
