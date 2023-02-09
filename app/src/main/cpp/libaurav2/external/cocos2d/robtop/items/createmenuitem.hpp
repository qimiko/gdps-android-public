#pragma once
#ifndef _ITEMS_CREATEMENUITEM_HPP
#define _ITEMS_CREATEMENUITEM_HPP

#include <robtop/sprites/ccmenuitemspriteextra.hpp>

class CreateMenuItem : public CCMenuItemSpriteExtra {
public:
    void setPage(int);
    void setTag(int);
    void setObjectKey(int);
    void setObjectFrame(std::string);
};

#endif
