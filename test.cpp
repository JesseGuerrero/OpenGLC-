#include <fstream>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>
#include "jesse.h"


int main(int argc, char** argv)
{
    std::cout << ReadFileJesse("ShadersFragment.glsl") << std::endl;
    return 0;
}