#pragma once
#ifndef _LAYERS_BOOMSCROLLLAYER_HPP
#define _LAYERS_BOOMSCROLLLAYER_HPP

class BoomScrollLayer : public cocos2d::CCLayer {
public:
	void addPage(cocos2d::CCLayer*, int);
};

#endif
