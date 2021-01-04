//
// Created by qianchen on 2020/11/17.
//

#include "../include/Fcb.h"


bool initFcb(Fcb & fcb,const char mfilename[], const char mexname[],
             unsigned char mattribute,unsigned short mtime,
             unsigned short mdate,int mfirst,
             unsigned long mlength,char mfree)
{
    if((strlen(mfilename)<8)&&(strlen(mexname)<3))
    {
        memcpy(fcb.filename,mfilename,strlen(mfilename)+1);
        memcpy(fcb.exname,mexname,strlen(mfilename)+1);
        fcb.attribute = mattribute;
        fcb.time = mtime;
        fcb.date = mdate;
        fcb.first = mfirst;
        fcb.length = fcb.attribute?mlength:0;
        fcb.free = mfree;

        return true;
    }
    else
    {
        cout << "[FCB]The filename of exname is too long!" << endl;
        return false;
    }

}