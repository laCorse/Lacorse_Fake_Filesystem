//
// Created by qianchen on 2020/11/30.
//

#ifndef LACORSE_FAKE_FILESYS_COMMANDLINE_H
#define LACORSE_FAKE_FILESYS_COMMANDLINE_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>

#include "Filesystem.h"
using namespace std;

/**
 * @classes 以CommandBase为基类的一系列具体指令
 * @pattern 策略模式
 */

//!命令行的模板
class CommandBase
{
//private无法直接继承
protected:
    vector<string> parameter;
    shared_ptr<Filesystem> fakeFs;


public:
    CommandBase(vector<string> cmdPara,shared_ptr<Filesystem> fs):parameter(cmdPara),fakeFs(fs)
    {}

    virtual void Execute()=0;
    virtual ~CommandBase(){};



    /**
     * @brief 在指定目录下创建文件
     * @param filename 名字
     * @param attribute 指定文件类型是文件还是目录
     * @return
     */
    int touch(string & filename, int attribute);

    /**
     * @brief 打开一个文件并读入到当前打开文件中,并分配一个openfilelist的下标作为描述符
     */
    int open(string & filename);


};


class Empty :public CommandBase
{

public:
    Empty(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute()
    {
        cout << "[LaSH]Command not found!"<<endl;
    }
};

/**
 * @todo 实现具体指令
 */
class Mkdir :public CommandBase
{

public:
    Mkdir(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute();
};

class Cd :public CommandBase
{

public:
    Cd(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute();
};

class Rmdir :public CommandBase
{

public:
    Rmdir(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute();
};

class Ls :public CommandBase
{

public:
    Ls(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute();
};

class Create :public CommandBase
{

public:
    Create(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute();
};

class Rm :public CommandBase
{

public:
    Rm(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute();
};

class Open :public CommandBase
{

public:
    Open(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute();
};

class Close :public CommandBase
{

public:
    Close(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute();
};

class Write :public CommandBase
{

public:
    Write(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute();
};

class Exit :public CommandBase
{

public:
    Exit(vector<string> cmdPara,shared_ptr<Filesystem> fs):CommandBase(cmdPara, fs)
    {}

    virtual void Execute();
};

#endif //LACORSE_FAKE_FILESYS_COMMANDLINE_H
