//
// Created by qianchen on 2020/11/17.
//

#include "../include/Useropen.h"


bool Useropen::operator==(Useropen & otherOpen)
{
    if (this->dir.size()!=otherOpen.dir.size())
        return false;
    for (int i = 0; i < this->dir.size(); ++i)
    {
        if (this->dir[i]!=otherOpen.dir[i])
            return false;
    }
    return true;
}


