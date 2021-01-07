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
    bool isRoot = false;
    bool isAbosolute = true;

    string path = parameter[0];

    if (path[0] == '/')
    {
        isRoot = true;
    }
    else
    {
        isRoot = false;
    }

    vector<string> file_paths = stringSplit(path,"/");
    if (!isRoot)
    {
        if (file_paths[0] == ".")
            isAbosolute = false;
        if(!isAbosolute)
        {
            vector<string> tmpPath{fakeFs->GetCurrentData().path};
            tmpPath.insert(tmpPath.end(),file_paths.begin()+1,file_paths.end());
            if(!fakeFs->make_dir(tmpPath))
            {
                cout<<"[Err]Cannot mkdir!"<<endl;
                return;
            }
        }
        else
        {
            cout<<"[Err]Cannot mkdir!"<<endl;
            return;
        }
    }
    else
    {
        if(!fakeFs->make_dir(file_paths))
        {
            cout<<"[Err]Cannot mkdir!"<<endl;
            return;
        }
    }


}

/**
 * @brief 将当前目录改为指定目录
 * @todo 1.打开并读入当前目录，关闭原有目录文件 2.修改当前目录id 3.更新当前目录名（路径）currentdir的值
 */
void Cd::Execute()
{
    bool isRoot = false;
    bool isAbosolute = true;

    string path = parameter[0];

    if (path[0] == '/')
    {
        isRoot = true;
    }
    else
    {
        isRoot = false;
    }

    vector<string> file_paths = stringSplit(path,"/");
    int id;
    if (!isRoot)
    {
//        if (file_paths[0] == ".")
//            isAbosolute = false;
//        if(!isAbosolute)
//        {
//            vector<string> tmpPath{fakeFs->GetCurrentData().path};
//            tmpPath.insert(tmpPath.end(),file_paths.begin()+1,file_paths.end());
//            if(!fakeFs->make_dir(tmpPath))
//            {
//                cout<<"[Err]Cannot mkdir!1"<<endl;
//                return;
//            }
//        }
//        else
//        {
//            cout<<"[Err]Cannot mkdir!2"<<endl;
//            return;
//        }
    }
    else
    {
        if(!fakeFs->search_file(file_paths,id))
        {
            cout<<"[Err]Cannot cd there!"<<endl;
            return;
        }
        else
        {
            cout <<"search id: "<<id<<endl;
            fakeFs->read_dir(id,file_paths);
        }
    }
}

void Rmdir::Execute()
{

}

void Ls::Execute()
{

    cout << "current blockId: "<<fakeFs->GetCurrentData().currentFileId <<endl;
    fakeFs->read_dir(fakeFs->GetCurrentData().currentFileId,fakeFs->GetCurrentData().path);
    cout << "current blockId...: "<<fakeFs->GetCurrentData().currentFileId <<endl;


    vector<Fcb> fcbList(fakeFs->GetCurrentData().currentDoc.fcbList);
    //cout << "current: " << fakeFs->GetCurrentData().currentFileId<<"size: "<< fcbList.size()<<endl;
    for (int i = 0; i < fcbList.size(); ++i) {
        cout << fcbList[i].filename << fcbList[i].exname << "\t";
    }
    cout<<endl;
}

//
void Create::Execute()
{
    bool isRoot = false;
    bool isAbosolute = true;

    string path = parameter[0];

    if (path[0] == '/')
    {
        isRoot = true;
    }
    else
    {
        isRoot = false;
    }

    vector<string> file_paths = stringSplit(path,"/");
    if (!isRoot)
    {
    }
    else
    {
        if(!fakeFs->make_file(file_paths))
        {
            cout<<"[Err]Cannot mkdir!"<<endl;
            return;
        }
    }
}

void Rm::Execute()
{

}

void Open::Execute()
{

    bool isRoot = false;
    bool isAbosolute = true;
    string path = parameter[0];
    if (path[0] == '/')
    {
        isRoot = true;
    }
    else
    {
        isRoot = false;
    }

    vector<string> file_paths = stringSplit(path,"/");
    if (!isRoot)
    {

    }
    else
    {
        //TODO cout the fd!
        cout << "[]The open file's fd is "<<fakeFs->add_openFile(file_paths)<<endl;
    }

}

void Close::Execute()
{

}


void Write::Execute()
{
    int mode=-1;
    if(parameter[0] == string("-c")||parameter[0] == string("--cut"))
    {
        mode = CUT;
    }
    else if(parameter[0] == string("-co")||parameter[0] == string("--cover"))
    {
        mode = COVER;
    }
    else if(parameter[0] == string("-a")||parameter[0] == string("--add"))
    {
        mode = ADD;
    }
    else
    {
        cout << "[Err]Cannot use this command!"<<endl;
        return;
    }


    ///show
    stringstream ss;
    string tmp = parameter[1];
    ss << tmp;int fd;ss >> fd;


    if (fd>=MAXOPENFILE||fd<0)
    {
        cout << "[Err]Cannot use this fd!"<<endl;
        return;
    }

    if (!fakeFs->get_openFile(fd).empty())
        fakeFs->get_openFile(fd).read();

    ///pool
    //char pool[BLOCKSIZE*10];
    //test
    string stmp;
    cin >> stmp;
    ///write
    fakeFs->get_openFile(fd).write(mode, const_cast<char *>(stmp.c_str()));



}


