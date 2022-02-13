#pragma once
#ifndef _OBJECTS_SONGINFOOBJECT_HPP
#define _OBJECTS_SONGINFOOBJECT_HPP

#include <cocos2d.h>
#include <string>

class SongInfoObject : public cocos2d::CCNode {
public:
    int getSongID() const;
    std::string getArtistName() const;
    std::string getSongName() const;
};

#endif
