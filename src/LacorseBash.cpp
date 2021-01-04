//
// Created by qianchen on 2020/11/30.
//

#include "../include/LacorseBash.h"

string vecs2s(vector<string> paths)
{
    string s{};
    for (int i = 0; i < paths.size(); ++i) {
        s+=paths[i];
        if (i!=paths.size()-1)
            s+='/';
    }
    return s;
}

void LacorseBash::Show()
{
    //输出的样子
    cout<<username<<'@'<<username<<':'<<vecs2s((fakeFs->GetCurrentData()).path)<<"$ ";

}

void LacorseBash::Read()
{
    command.clear();
    parameter.clear();
    //读入指令,使用string流
    string cmdLine;
    getline(cin,cmdLine);
    istringstream cmdLineStream(cmdLine);
    //获取指令
    cmdLineStream >> command;
    if (command == "exit") exit(0);
    string tmpSs;
    //获取参数
    while (cmdLineStream>>tmpSs)
        parameter.push_back(tmpSs);

    //更新命令
    cmd = cmdFactory->SmartCreateCmd(command,parameter,fakeFs);
}

void LacorseBash::Run()
{
    //构造死循环
    alwaysTrue = true;
    while (alwaysTrue)
    {
        //展示用户信息
        Show();
        //读取（更新指令信息）
        Read();
        //执行
        cmd -> Execute();
    }
}