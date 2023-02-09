#pragma once
#ifndef _EXTENSIONS_DS_DICTIONARY_HPP
#define _EXTENSIONS_DS_DICTIONARY_HPP

#include <cocos2d.h>

class DS_Dictionary {
public:
    bool getBoolForKey(const char*);
    float getFloatForKey(const char*);
    int getIntegerForKey(const char*);
    std::string getKey(const char*);
    std::string getStringForKey(const char*);

    void setBoolForKey(const char*, bool, bool alphaNumericallySorted);
    void setBoolForKey(const char*, bool);

    void setFloatForKey(const char*, float, bool alphaNumericallySorted);
    void setFloatForKey(const char*, float);

    void setIntegerForKey(const char*, int, bool alphaNumericallySorted);
    void setIntegerForKey(const char*, int);

    void setStringForKey(const char*, std::string const&,
        bool alphaNumericallySorted);
    void setStringForKey(const char*, std::string const&);

    cocos2d::CCArray* getArrayForKey(const char*);
    void setArrayForKey(const char*, cocos2d::CCArray*);

    cocos2d::CCObject* getObjectForKey(const char*);
    void setObjectForKey(const char*, cocos2d::CCObject*);

    bool loadRootSubDictFromFile(const char* filename);
    bool loadRootSubDictFromString(std::string string);
    bool loadRootSubDictFromCompressedFile(const char* filename);
};

#endif
