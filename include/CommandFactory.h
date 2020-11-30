//
// Created by qianchen on 2020/11/30.
//

#ifndef LACORSE_FAKE_FILESYS_COMMANDFACTORY_H
#define LACORSE_FAKE_FILESYS_COMMANDFACTORY_H

#include "Commandline.h"


/**
 * @class   指令工厂
 * @pattern 简单工厂模式
 * @todo 考虑可以改成具体工厂模式，这样的if...else...太丑！
 */
class CommandFactory
{
private:

public:

    unique_ptr<CommandBase> SmartCreateCmd(string command,vector<string> parameter,shared_ptr<Filesystem> fs)
    {
        if (command == "mkdir")
            return unique_ptr<CommandBase>(new Mkdir(parameter,fs));
        else if(command == "rmdir")
            return unique_ptr<CommandBase>(new Rmdir(parameter,fs));
        else if(command == "cd")
            return unique_ptr<CommandBase>(new Cd(parameter,fs));
        else if(command == "ls")
            return unique_ptr<CommandBase>(new Ls(parameter,fs));
        else if(command == "create")
            return unique_ptr<CommandBase>(new Create(parameter,fs));
        else if(command == "rm")
            return unique_ptr<CommandBase>(new Rm(parameter,fs));
        else if(command == "open")
            return unique_ptr<CommandBase>(new Open(parameter,fs));
        else if(command == "close")
            return unique_ptr<CommandBase>(new Close(parameter,fs));
        else if(command == "write")
            return unique_ptr<CommandBase>(new Write(parameter,fs));
        else
            return unique_ptr<CommandBase>(new Empty(parameter,fs));

    }
};

#endif //LACORSE_FAKE_FILESYS_COMMANDFACTORY_H
