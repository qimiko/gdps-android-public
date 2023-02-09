#pragma once
#ifndef _LAYERS_SHARELEVELSETTINGSLAYER_HPP
#define _LAYERS_SHARELEVELSETTINGSLAYER_HPP

#include <robtop/objects/gjgamelevel.hpp>

#include "flalertlayer.hpp"

class ShareLevelSettingsLayer : public FLAlertLayer {
public:
	bool init(GJGameLevel* lvl);
};

#endif
