#include <iostream>
#include "include/common.h"
#include "include/Filesystem.h"
#include "include/LacorseBash.h"

int main() {

    unique_ptr<LacorseBash> lacorseBash(new LacorseBash);
    lacorseBash->Run();

    return 0;
}
