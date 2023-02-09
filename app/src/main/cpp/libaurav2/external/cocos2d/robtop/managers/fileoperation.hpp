#pragma once
#ifndef _MANAGERS_FILEOPERATION_HPP
#define _MANAGERS_FILEOPERATION_HPP

#include <string>

class FileOperation {
public:
    static std::string getFilePath();
    static void readFile();
    static void saveFile();
};

#endif