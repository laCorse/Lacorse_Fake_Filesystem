//
// Created by qianchen on 2020/11/30.
//

#ifndef LACORSE_FAKE_FILESYS_LACORSEBASH_H
#define LACORSE_FAKE_FILESYS_LACORSEBASH_H

#include <string>
#include <sstream>
#include <vector>
#include "Commandline.h"
#include "CommandFactory.h"
#include "Filesystem.h"


extern "C"
{
#include <pwd.h>
#include <unistd.h>
#include <cstdlib>
};

class LacorseBash
{
public:
    LacorseBash():fakeFs(new Filesystem)
    {
        //获取当前用户信息
        struct passwd* pwd = getpwuid(getuid());
        username = string(pwd->pw_name);
    }

    ~LacorseBash(){}

    /**
     * @brief 用于在标准输出中展示用户名等
     */
    void Show();

    /**
     * @brief 用于读入命令和相应参数
     */
    void Read();


    /**
     * @brief 实现Bash完整流程：读入、执行相应命令
     */
    void Run();

private:
    //!需要指向一个文件系统
    shared_ptr<Filesystem> fakeFs;
    //!指令行
    unique_ptr<CommandBase> cmd;
    //!命令工厂
    unique_ptr<CommandFactory> cmdFactory;
    //!命令相关(指令和参数)
    string command;
    vector<string> parameter;
    //!当前用户名
    string username;


    //!用来表示永远为True（主要是死循环IDE会警告看着太烦）
    bool alwaysTrue;

};

#endif //LACORSE_FAKE_FILESYS_LACORSEBASH_H
