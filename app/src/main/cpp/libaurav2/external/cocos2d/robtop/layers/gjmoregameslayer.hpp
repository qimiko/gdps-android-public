#pragma once
#ifndef _LAYERS_GJMOREGAMESLAYER_HPP
#define _LAYERS_GJMOREGAMESLAYER_HPP

#include "gjdropdownlayer.hpp"
#include <cocos2d.h>

class GJMoreGamesLayer : public GJDropDownLayer {
public:
    cocos2d::CCArray* getMoreGamesList();
};

#endif
