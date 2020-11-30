//
// Created by qianchen on 2020/11/30.
//

#include "../include/Commandline.h"

/**
 * @brief 在当前目录或指定目录下创建名为dirname子目录
 * @todo 1.查重 2.分配空间 3.初始化子目录，在其中添加"."和“..”
 */
void Mkdir::Execute()
{
    currentData curData = fakeFs ->GetCurrentData();

}

/**
 * @brief 将当前目录改为指定目录
 * @todo 1.打开并读入当前目录，关闭原有目录文件 2.修改当前目录id 3.更新当前目录名（路径）currentdir的值
 */
void Cd::Execute()
{

}

void Rmdir::Execute()
{

}

void Ls::Execute()
{

}

void Create::Execute()
{

}

void Rm::Execute()
{

}

void Open::Execute()
{

}

void Close::Execute()
{

}

void Write::Execute()
{

}


