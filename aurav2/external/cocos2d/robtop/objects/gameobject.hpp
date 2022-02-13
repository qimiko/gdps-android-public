#pragma once
#ifndef _OBJECTS_GAMEOBJECT_HPP
#define _OBJECTS_GAMEOBJECT_HPP

#include <cocos2d.h>

#include <robtop/sprites/ccspriteplus.hpp>

class GameObject : public CCSpritePlus {
public:
    bool flipX;
    bool flipY;

    cocos2d::CCPoint unknown_0x1C4;
    cocos2d::CCObject* orientedBox;
    bool isOriented;

    cocos2d::CCObject* objectGlow;
    bool isSpeedPortal;
    bool isRingObject;

    cocos2d::CCAction* myAction;
    bool isRotatingObject;
    bool poweredOn;
    bool hideObject;

    float unknown_0x1E4;
    float unknown_0x1E8;

    bool isActive;
    bool unknown_0x1ED;
    bool isDestroyed;

    cocos2d::CCParticleSystemQuad* objectParticleSystem;
    std::string visibilityString;
    bool isVisible;

    cocos2d::CCPoint unknown_0x1FC;
    cocos2d::CCRect unknown_0x204;

    float fadeInPosOffset;
    cocos2d::CCRect unknown_0x21C;

    bool objectRectDirty;
    bool isStartPos;
    bool hasBeenActivatedByPlayer;
    bool hasBeenActivated;
    bool canChangeCustomColor;
    bool hasCustomChildren;
    bool hasColorSprite;

    cocos2d::CCObject* colorSprite;
    bool ignoreScreenCheck;

    float radius;
    bool isRotated;

    float scaleModX;
    float scaleModY;

    int M_ID;

    GameObjectType gameObjectType;
    int sectionIdx;
    bool shouldSpawn;
    bool touchTriggered;

    cocos2d::CCPoint realPosition;
    std::string realTextureName;

    bool isDisabled;
    bool useAudioScale;
    bool isSleeping;

    float startRotation;
    float startScaleX;
    float startScaleY;
    bool startFlipX;
    bool startFlipY;
    bool shouldHide;

    float spawnXPos;
    bool isInvisible;

    float enterAngle;
    int enterEffect;

    cocos2d::ccColor3B tintColor;
    float tintDuration;
    bool tintGround;

    int objectKey;
    bool dontTransform;
    bool dontFade;
    bool dontFadeTinted;
    bool isTintObject;
    bool stateVar;

    int objectZ;
    cocos2d::CCNode* parentObject;

    bool customAudioScale;
    float minAudioScale;
    float maxAudioScale;

    int uniqueID;
    bool invisibleMode;

    bool glowUseBGColor;
    bool useSpecialLight;
    bool glowUseReverseColor;
    bool isColorObject;

    float opacityMod;
    float glowOpacityMod;

    bool dontShow;
    bool upSlope;

    int slopeType;
    float slopeAngle;

    bool isDangerousObject;
    GJCustomColorMode customColorMode;
    GJCustomColorMode colorMode;

    bool forceBottomLayer;
    bool editorSelected;
    bool copyPlayerColor1;
    bool copyPlayerColor2;
    bool customColorBlend;
    bool wasSelected;
    bool isSelected;

    int editorSelectIdx;
    cocos2d::CCPoint storedPosition;
    int groupID;

    static GameObject* create(const char*);
    void customSetup();
};

#endif
