cmake_minimum_required(VERSION 3.16)
project(Lacorse_Fake_FileSys)

set(CMAKE_CXX_STANDARD 17)

add_executable(Lacorse_Fake_FileSys main.cpp include/common.h src/Fcb.cpp include/Fcb.h src/Useropen.cpp
        include/Useropen.h include/Fat.h include/BLOCK0.h include/Filesystem.h src/Filesystem.cpp src/Commandline.cpp include/Commandline.h src/LacorseBash.cpp include/LacorseBash.h include/CommandFactory.h)