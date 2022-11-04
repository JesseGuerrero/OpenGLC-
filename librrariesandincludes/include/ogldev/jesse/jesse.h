//
// Created by jesse on 028, Oct 28.
//

#ifndef CPPOPENGL_JESSE_H
#define CPPOPENGL_JESSE_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

std::string ReadFileJesse(std::string path) {
    std::ifstream t(path);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

#endif //CPPOPENGL_JESSE_H
