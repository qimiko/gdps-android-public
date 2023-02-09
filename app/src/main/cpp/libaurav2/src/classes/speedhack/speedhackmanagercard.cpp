#include "classes/speedhack/speedhackmanagercard.hpp"

bool SpeedhackManagerCard::gHackActive_ = false;
int SpeedhackManagerCard::gLifetimeCounter_ = 0;

float SpeedhackManagerCard::getIntervalForValue(float value) {
    float val = 1.0f + (value * 0.25f);
    return std::min(std::max(val, 0.1f), 3.0f);
}

int SpeedhackManagerCard::getSaveValueForInterval(int old_val, int change) {
    if (getIntervalForValue(old_val) == getIntervalForValue(old_val + change)) {
        return old_val;
    } else {
        return old_val + change;
    }
}

void SpeedhackManagerCard::setGlobalTimeScale(float scale) {
    cocos2d::CCDirector::sharedDirector()->getScheduler()->setTimeScale(scale);
}

void SpeedhackManagerCard::setSpeedhackActive(bool active) {
    gHackActive_ = active;
}

void SpeedhackManagerCard::decrementLifetime() {
    gLifetimeCounter_--;
    if (gLifetimeCounter_ <= 0) {
        gLifetimeCounter_ = 0;

        setSpeedhackActive(false);
        setSpeedhackValue(1.0f);
    }

    spdlog::get("global")->trace("speedhack lifecycle ended, {}", gLifetimeCounter_);
}

void SpeedhackManagerCard::incrementLifetime() {
    gLifetimeCounter_++;
    setSpeedhackActive(true);

    spdlog::get("global")->trace("speedhack lifecycle begun, {}", gLifetimeCounter_);
}

bool SpeedhackManagerCard::isSpeedhackActive() {
    return gHackActive_;
}

float SpeedhackManagerCard::getSpeedhackInterval() {
    auto gm = GameManager::sharedState();
    auto speedhack_interval = gm->getIntGameVariable(GameVariable::SPEED_INTERVAL);

    return getIntervalForValue(speedhack_interval);
}

void SpeedhackManagerCard::setSpeedhackValue(float interval) {
    // if (!GameManager::sharedState()->getGameVariable(GameVariable::SPEED_CONTROLS)) {
    //     setSpeedhackActive(false);
    //     return;
    // }

    if (auto pl = GameManager::sharedState()->getPlayLayer(); pl != nullptr) {
        if (auto pl_ext = dynamic_cast<PlayLayerExt *>(pl->getUserObject());
                pl_ext != nullptr) {
            if (interval != 1.0f) {
                pl_ext->setIsIllegitimate(true);
            }
        }
    }

    // enables speedhack if interval is not 1
    setSpeedhackActive(interval != 1.0f);
    setGlobalTimeScale(interval);
}

void SpeedhackManagerCard::setExternalLabel(cocos2d::CCLabelBMFont* label) {
    this->externalLabel_ = label;
}

void SpeedhackManagerCard::updateSpeedhackLabel() {
    auto speedhack_interval = getSpeedhackInterval();
    auto speedhack_string = fmt::format("{:.2f}x", speedhack_interval);

    optionsLabel_->setString(speedhack_string.c_str(), true);

    if (externalLabel_) {
        externalLabel_->setString(speedhack_string.c_str(), true);
    }

    setSpeedhackValue(speedhack_interval);
}

void SpeedhackManagerCard::onBtnDown(cocos2d::CCObject * /* target */) {
    auto gm = GameManager::sharedState();
    auto speedhack_interval = gm->getIntGameVariable(GameVariable::SPEED_INTERVAL);

    gm->setIntGameVariable(GameVariable::SPEED_INTERVAL,
                           getSaveValueForInterval(speedhack_interval, -1));

    this->updateSpeedhackLabel();
}

void SpeedhackManagerCard::onBtnUp(cocos2d::CCObject * /* target */) {
    auto gm = GameManager::sharedState();
    auto speedhack_interval = gm->getIntGameVariable(GameVariable::SPEED_INTERVAL);

    gm->setIntGameVariable(GameVariable::SPEED_INTERVAL,
                           getSaveValueForInterval(speedhack_interval, 1));

    this->updateSpeedhackLabel();
}

bool SpeedhackManagerCard::init() {
    auto time_sprite = cocos2d::CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
    this->addChild(time_sprite);
    time_sprite->setPosition(
            cocos2d::CCPoint(-25.0f, 0.0f));

    auto items_menu = cocos2d::CCMenu::create();
    this->addChild(items_menu);
    items_menu->setPosition(0.0f, 0.0f);

    // arrow begins pointing up
    auto speedhack_down = cocos2d::CCSprite::createWithSpriteFrameName(
            "navArrowBtn_001.png");
    speedhack_down->setFlipY(true);
    speedhack_down->setRotation(90.0f);
    speedhack_down->setScale(0.8f);

    auto speedhack_up = cocos2d::CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
    speedhack_up->setFlipX(true);
    speedhack_up->setFlipY(true);
    speedhack_up->setRotation(90.0f);
    speedhack_up->setScale(0.8f);

    auto speedhack_down_btn = CCMenuItemSpriteExtra::create(
            speedhack_down, nullptr, this,
            static_cast<cocos2d::SEL_MenuHandler>(
                    &SpeedhackManagerCard::onBtnDown));
    items_menu->addChild(speedhack_down_btn);
    speedhack_down_btn->setPositionY(-30.0f);

    auto speedhack_up_btn = CCMenuItemSpriteExtra::create(
            speedhack_up, nullptr, this,
            static_cast<cocos2d::SEL_MenuHandler>(
                    &SpeedhackManagerCard::onBtnUp));
    items_menu->addChild(speedhack_up_btn);
    speedhack_up_btn->setPositionY(30.0f);

    auto speedhack_interval = getSpeedhackInterval();
    auto speedhack_string = cocos2d::CCString::createWithFormat("%.2fx",
                                                                speedhack_interval);

    optionsLabel_ = cocos2d::CCLabelBMFont::create(
            speedhack_string->getCString(), "bigFont.fnt");
    this->addChild(optionsLabel_);

    optionsLabel_->setPosition(15.0f, 0.0f);
    optionsLabel_->setScale(0.5f);

    return true;
}
