#pragma once
#ifndef _LAYERS_SHARELEVELLAYER_HPP
#define _LAYERS_SHARELEVELLAYER_HPP

#include <robtop/objects/gjgamelevel.hpp>

#include "flalertlayer.hpp"

class ShareLevelLayer : public FLAlertLayer {
public:
    bool init(GJGameLevel*);
};

#endif
