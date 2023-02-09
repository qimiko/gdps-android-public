#pragma once
#ifndef _OBJECTS_GAMEOBJECT_HPP
#define _OBJECTS_GAMEOBJECT_HPP

#include <cocos2d.h>

#include <robtop/sprites/ccspriteplus.hpp>

class GameObject : public CCSpritePlus {
public:
    virtual void resetObject();
    virtual void triggerObject();
    virtual void activateObject();
    virtual void deactivateObject();

    virtual cocos2d::CCRect getObjectRect();
    virtual cocos2d::CCRect getObjectRect(float, float);
    virtual cocos2d::CCRect getObjectRect2(float, float);
    virtual cocos2d::CCRect getObjectTextureRect();

    virtual cocos2d::CCPoint getRealPosition();

    virtual void setStartPos(cocos2d::CCPoint);
    virtual void updateStartValues();

    virtual std::string getSaveString();

    virtual bool isFlipX();
    virtual bool isFlipY();

    virtual void setRScaleX(float);
    virtual void setRScaleY(float);
    virtual void setRScale(float);
    virtual float getRScaleX();
    virtual float getRScaleY();

    virtual void calculateSpawnXPos();

    virtual bool triggerActivated();
    virtual void powerOnObject();
    virtual void powerOffObject();

    // real type: OBB2D*
    virtual cocos2d::CCObject* getOrientedBox();

    void setObjectColor(cocos2d::ccColor3B);
    void setChildColor(cocos2d::ccColor3B);

    bool addToBottom();

private:
    bool flipX_;
    bool flipY_;

    cocos2d::CCPoint unknown_0x1C4_;

    // real type: OBB2D*
    cocos2d::CCObject* orientedBox_;

    CC_SYNTHESIZE_READONLY(bool, isOriented_, IsOriented);

private:
    cocos2d::CCObject* objectGlow_;
    bool isSpeedPortal_;
    bool isRingObject_;

    cocos2d::CCAction* myAction_;
    bool isRotatingObject_;
    bool poweredOn_;

    CC_SYNTHESIZE_READONLY(bool, hideObject_, HideObject);

private:
    float unknown_0x1E4_;
    float unknown_0x1E8_;

    CC_SYNTHESIZE_READONLY(bool, isActive_, IsActive);

private:
    bool hasGlow_;
    bool isDestroyed_;

    cocos2d::CCParticleSystemQuad* objectParticleSystem_;
    std::string particleString_;
    bool particlesVisible_;

    cocos2d::CCPoint unknown_0x1FC;
    cocos2d::CCRect calculatedObjectTextureRect_;
    bool objectTextureRectDirty_;

    CC_SYNTHESIZE(float, fadeInPosOffset_, FadeInPosOffset);

private:
    cocos2d::CCRect calculatedObjectRect2_;

    CC_SYNTHESIZE(float, objectRectDirty_, ObjectRectDirty);

private:
    bool orientedBoxDirty_;

    bool activatedByPlayer_;
    bool activated_;

    bool canChangeCustomColor_;

    CC_SYNTHESIZE_READONLY(bool, hasCustomChildren_, HasCustomChildren);
    CC_SYNTHESIZE_READONLY(bool, hasColor_, HasColor);
    CC_SYNTHESIZE_READONLY(cocos2d::CCSprite*, colorSprite_, ColorSprite);

    CC_SYNTHESIZE(bool, ignoreScreenCheck_, IgnoreScreenCheck);

    CC_SYNTHESIZE_READONLY(float, radius_, Radius);
    CC_SYNTHESIZE_READONLY(bool, isRotated_, IsRotated);

    CC_SYNTHESIZE(float, scaleModX_, ScaleModX);
    CC_SYNTHESIZE(float, scaleModY_, ScaleModY);

    CC_SYNTHESIZE(int, M_ID_, M_ID);

    CC_SYNTHESIZE(GameObjectType, objectType_, Type);

    CC_SYNTHESIZE(int, sectionIdx_, SectionIdx);

    CC_SYNTHESIZE_READONLY(bool, shouldSpawn_, ShouldSpawn);

    CC_SYNTHESIZE(int, touchTriggered_, TouchTriggered);

    CC_SYNTHESIZE_READONLY(cocos2d::CCPoint, spawnPosition_, StartPos);

    CC_SYNTHESIZE_READONLY(std::string, frame_, Frame);

    CC_SYNTHESIZE(bool, isDisabled_, IsDisabled);

    CC_SYNTHESIZE(bool, useAudioScale_, UseAudioScale);

    CC_SYNTHESIZE_READONLY(bool, isSleeping_, IsSleeping);

    CC_SYNTHESIZE(float, startRotation_, StartRotation);
    CC_SYNTHESIZE(float, startScaleX_, StartScaleX);
    CC_SYNTHESIZE(float, startScaleY_, StartScaleY);
    CC_SYNTHESIZE(bool, startFlipX_, StartFlipX);
    CC_SYNTHESIZE(bool, startFlipY_, StartFlipY);

    CC_SYNTHESIZE(bool, shouldHide_, ShouldHide);

    CC_SYNTHESIZE_READONLY(float, spawnXPos_, SpawnXPos);

    CC_SYNTHESIZE_READONLY(bool, isInvisible_, IsInvisible);

    CC_SYNTHESIZE(float, enterAngle_, EnterAngle);
    CC_SYNTHESIZE(int, enterEffect_, EnterEffect);

    CC_SYNTHESIZE(cocos2d::ccColor3B, tintColor_, TintColor);
    CC_SYNTHESIZE(float, tintDuration_, TintDuration);
    CC_SYNTHESIZE(bool, tintGround_, TintGround);

    CC_SYNTHESIZE(int, objectKey_, ObjectKey);

    CC_SYNTHESIZE(bool, dontTransform_, DontTransform);
    CC_SYNTHESIZE(bool, dontFade_, DontFade);
    CC_SYNTHESIZE(bool, dontFadeTinted_, DontFadeTintend);

    CC_SYNTHESIZE(bool, isTintObject_, IsTintObject);

    CC_SYNTHESIZE(bool, stateVar_, StateVar);

    CC_SYNTHESIZE(int, objectZ_, ObjectZ);
    CC_SYNTHESIZE(cocos2d::CCNode*, objectParent_, ObjectParent);

    CC_SYNTHESIZE_READONLY(bool, customAudioScale_, CustomAudioScale);
    CC_SYNTHESIZE_READONLY(float, minAudioScale_, MinAudioScale);
    CC_SYNTHESIZE_READONLY(float, maxAudioScale_, MaxAudioScale);

    CC_SYNTHESIZE(int, uniqueID_, UniqueID);

    CC_SYNTHESIZE_READONLY(bool, invisibleMode_, InvisibleMode);
    CC_SYNTHESIZE_READONLY(bool, glowUseBGColor_, GlowUseBGColor);
    CC_SYNTHESIZE_READONLY(bool, useSpecialLight_, UseSpecialLight);
    CC_SYNTHESIZE_READONLY(bool, glowUseReverseColor_, GlowUseReverseColor);
    CC_SYNTHESIZE_READONLY(bool, isColorObject_, IsColorObject);

    CC_SYNTHESIZE(float, opacityMod_, OpacityMod);
    CC_SYNTHESIZE(float, glowOpacityMod_, GlowOpacityMod);

    CC_SYNTHESIZE(bool, dontShow_, DontShow);

    CC_SYNTHESIZE_READONLY(bool, upSlope_, UpSlope);
    CC_SYNTHESIZE_READONLY(int, slopeType_, SlopeType);
    CC_SYNTHESIZE_READONLY(float, slopeAngle_, SlopeAngle);

    CC_SYNTHESIZE_READONLY(bool, isDangerousObject_, Damaging);

    CC_SYNTHESIZE_READONLY(GJCustomColorMode, customColorMode_, CustomColorMode);

    bool shouldBlend();
    bool shouldBlendColor();
    GJCustomColorMode getColorMode();

private:
    GJCustomColorMode defaultColorMode_;

    CC_SYNTHESIZE(bool, forceBottomLayer_, ForceBottomLayer);

    CC_SYNTHESIZE(bool, editorSelected_, EditorSelected);

    CC_SYNTHESIZE(bool, copyPlayerColor1_, CopyPlayerColor1);
    CC_SYNTHESIZE(bool, copyPlayerColor2_, CopyPlayerColor2);
    CC_SYNTHESIZE(bool, customColorBlend_, CustomColorBlend);

    CC_SYNTHESIZE(bool, wasSelected_, WasSelected);
    CC_SYNTHESIZE(bool, isSelected_, IsSelected);
    CC_SYNTHESIZE(int, editorSelectIdx_, EditorSelectIdx);
    CC_SYNTHESIZE(cocos2d::CCPoint, storedPosition, StoredPosition);
    CC_SYNTHESIZE(int, groupID_, GroupID);

public:
    static GameObject* create(const char*);
    void customSetup();
};

#endif
