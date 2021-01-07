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


bool Useropen::read()
{
    for(int i =0; i<file.size();i++)
    {
        cout << file[i]->read_content() << endl;
    }
    return true;
}

bool Useropen::setMode(int mode)
{
    if(mode == CUT)        ///截断
    {
        pos = 0;
    }
    else if(mode == COVER) ///覆盖
    {
        pos = pos;
    }
    else if (mode == ADD)  ///追加
    {
        pos = (file.size()-1)*BLOCKSIZE + strlen(file.back()->read_content().c_str());
    }
    else
    {
        return false;
    }
    return true;
}


int Useropen::lastBlockId()
{
    int last = userFcb.first;
    while ((*pFat)[last] != END)
    {
        file.push_back(new File(blocks[last]));
        last = (*pFat)[last];
    }
    return last;
}

bool Useropen::write(int mode,char * text)
{

    if(file.empty())
    {
        return false;
    }

    if(!setMode(mode))
    {
        return false;
    }
    // (BLOCKSIZE-tmpPos)                                (numTxt-BLOCKSIZE+tmpPos)%BLOCKSIZE
    //   |-----------|----------------------------------|---------|
    //tmpPos        newBlock
    //确定pos在哪个file
    int index = pos/BLOCKSIZE;
    int tmpPos = pos % BLOCKSIZE;

    //写入
    int numTxt = strlen(text);

    if(tmpPos+numTxt < BLOCKSIZE)///直接写
    {
        file.back()->change_content(text,tmpPos,numTxt);
        pos += numTxt;
    }
    else if(tmpPos+numTxt>BLOCKSIZE)///需要分配新块
    {
        int allocNum = (tmpPos+numTxt) / BLOCKSIZE;//分配的块数
        ///改fat表,并添加到file vector中
        int lastBlock = lastBlockId();
        int tmpNum = allocNum;
        while(tmpNum--)
        {
            int freeBlock = pFat->get_FreeBlock();
            (*pFat)[lastBlock] = freeBlock;
            (*pFat)[freeBlock] = END;
            file.push_back(new File(blocks[freeBlock]));
            lastBlock = freeBlock;
        }


        int firstNum = BLOCKSIZE - tmpPos;
        int lastNum = (numTxt-BLOCKSIZE+tmpPos)%BLOCKSIZE;
        int textPos = 0;
        for(int i = 0;i<allocNum;i++)
        {

            if (i==0)
            {
                file[index+i]->change_content(text,tmpPos,firstNum);
                textPos+=firstNum;
            }
            else if(i == allocNum-1)
            {
                file[index+i]->change_content(text+textPos,0,lastNum);
                textPos+=lastNum;
            }
            else
            {
                file[index+i]->change_content(text+textPos,0,BLOCKNUM);
                textPos+=BLOCKSIZE;
            }
        }
        pos += textPos;
    }

    cout << "pos : "<<pos <<endl;

    return true;




}

