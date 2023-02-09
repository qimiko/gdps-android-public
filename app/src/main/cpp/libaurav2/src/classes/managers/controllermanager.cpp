#include "classes/managers/controllermanager.hpp"

bool ControllerManager::isFloatNotCloseToZero(float value) {
    // ignore incredibly small values of the controller
    // this constant seems to be "good enough"
    return std::abs(value) > 0.05f;
}

void ControllerManager::update(float dt) {
    // update right joystick scroll
    if (isFloatNotCloseToZero(right_.axisX) || isFloatNotCloseToZero(right_.axisY)) {
        // they seem to be reversed for some reason? idk.
        constexpr auto SCROLL_FACTOR = 5.0f;

        cocos2d::CCDirector::sharedDirector()->getMouseDispatcher()
                ->dispatchScrollMSG(right_.axisY * SCROLL_FACTOR, -right_.axisX * SCROLL_FACTOR);
    }

    if (isFloatNotCloseToZero(left_.axisX) || isFloatNotCloseToZero(left_.axisY)) {
        // they seem to be reversed for some reason? idk.
        constexpr auto PUSH_FACTOR = 5.0f;

        controllerOverlay_->pushCursor(left_.axisX * PUSH_FACTOR, -left_.axisY * PUSH_FACTOR);
    }

    controllerOverlay_->update(dt);
}

bool ControllerManager::updateJoystickValues(const Joystick left, const Joystick right) {
    this->left_ = left;
    this->right_ = right;

    return true;
}

void ControllerManager::handleReset() {
    controllerOverlay_->resetCursor();
}

void ControllerManager::handleClickDown() {
    controllerOverlay_->pushDown();
}

void ControllerManager::handleClickUp() {
    controllerOverlay_->pushUp();
}

ControllerManager& ControllerManager::getManager() {
    static ControllerManager _managerInstance;
    return _managerInstance;
}

void ControllerManager::init() {
    // register with scheduler
    auto scheduler = cocos2d::CCDirector::sharedDirector()->getScheduler();
    scheduler->scheduleUpdateForTarget(this, 1, false);

    controllerOverlay_ = ControllerOverlay::create();
    cocos2d::CCDirector::sharedDirector()->setNotificationNode(controllerOverlay_);
}

ControllerManager::ControllerManager() {
    init();
};

ControllerManager::~ControllerManager() {
    auto scheduler = cocos2d::CCDirector::sharedDirector()->getScheduler();
    scheduler->unscheduleUpdateForTarget(this);
}

bool ControllerManager::isKeycodeControllerConfusable(int keyCode) {
    return keyCode == AKEYCODE_DPAD_LEFT
           || keyCode == AKEYCODE_DPAD_RIGHT
           || keyCode == AKEYCODE_DPAD_UP
           || keyCode == AKEYCODE_DPAD_DOWN;
}

cocos2d::enumKeyCodes ControllerManager::translateAndroidKeyCodeToWindows(int keyCode) {
    std::unordered_map<int, cocos2d::enumKeyCodes> codes = {
            {AKEYCODE_ESCAPE,        cocos2d::kCCEnumKeyCodesEscape},
            {AKEYCODE_SPACE,         cocos2d::kCCEnumKeyCodesSpace},
            {AKEYCODE_DPAD_LEFT,     cocos2d::kCCEnumKeyCodesLeft},
            {AKEYCODE_DPAD_RIGHT,    cocos2d::kCCEnumKeyCodesRight},
            {AKEYCODE_DPAD_UP,       cocos2d::kCCEnumKeyCodesUp},
            {AKEYCODE_DPAD_DOWN,     cocos2d::kCCEnumKeyCodesDown},
            {AKEYCODE_1,             cocos2d::kCCEnumKeyCodesOne},
            {AKEYCODE_2,             cocos2d::kCCEnumKeyCodesTwo},
            {AKEYCODE_3,             cocos2d::kCCEnumKeyCodesThree},
            {AKEYCODE_Q,             cocos2d::kCCEnumKeyCodesQ},
            {AKEYCODE_W,             cocos2d::kCCEnumKeyCodesW},
            {AKEYCODE_E,             cocos2d::kCCEnumKeyCodesE},
            {AKEYCODE_R,             cocos2d::kCCEnumKeyCodesR},
            {AKEYCODE_T,             cocos2d::kCCEnumKeyCodesT},
            {AKEYCODE_Y,             cocos2d::kCCEnumKeyCodesY},
            {AKEYCODE_A,             cocos2d::kCCEnumKeyCodesA},
            {AKEYCODE_S,             cocos2d::kCCEnumKeyCodesS},
            {AKEYCODE_D,             cocos2d::kCCEnumKeyCodesD},
            {AKEYCODE_F,             cocos2d::kCCEnumKeyCodesF},
            {AKEYCODE_G,             cocos2d::kCCEnumKeyCodesG},
            {AKEYCODE_Z,             cocos2d::kCCEnumKeyCodesZ},
            {AKEYCODE_X,             cocos2d::kCCEnumKeyCodesX},
            {AKEYCODE_C,             cocos2d::kCCEnumKeyCodesC},
            {AKEYCODE_V,             cocos2d::kCCEnumKeyCodesV},
            {AKEYCODE_F1,            cocos2d::kCCEnumKeyCodesF1},
            {AKEYCODE_F2,            cocos2d::kCCEnumKeyCodesF2},
            {AKEYCODE_ENTER,         cocos2d::kCCEnumKeyCodesEnter},
            {AKEYCODE_FORWARD_DEL,   cocos2d::kCCEnumKeyCodesDelete},
            {AKEYCODE_DEL,           cocos2d::kCCEnumKeyCodesBackspace},
            {AKEYCODE_CTRL_LEFT,     cocos2d::kCCEnumKeyCodesControl},
            {AKEYCODE_CTRL_RIGHT,    cocos2d::kCCEnumKeyCodesControl},
            {AKEYCODE_SHIFT_LEFT,    cocos2d::kCCEnumKeyCodesShift},
            {AKEYCODE_SHIFT_RIGHT,   cocos2d::kCCEnumKeyCodesShift},
            {AKEYCODE_ALT_LEFT,      cocos2d::kCCEnumKeyCodesAlt},
            {AKEYCODE_ALT_RIGHT,     cocos2d::kCCEnumKeyCodesAlt},
            {AKEYCODE_BUTTON_A,      cocos2d::kCCEnumKeyCodesControllerA},
            {AKEYCODE_BUTTON_B,      cocos2d::kCCEnumKeyCodesControllerB},
            {AKEYCODE_BUTTON_X,      cocos2d::kCCEnumKeyCodesControllerX},
            {AKEYCODE_BUTTON_Y,      cocos2d::kCCEnumKeyCodesControllerY},
            {AKEYCODE_BUTTON_START,  cocos2d::kCCEnumKeyCodesControllerStart},
            {AKEYCODE_BUTTON_SELECT, cocos2d::kCCEnumKeyCodesControllerBack},
            {AKEYCODE_BUTTON_R1,     cocos2d::kCCEnumKeyCodesControllerRB},
            {AKEYCODE_BUTTON_L1,     cocos2d::kCCEnumKeyCodesControllerLB},
            {AKEYCODE_BUTTON_R2,     cocos2d::kCCEnumKeyCodesControllerRT},
            {AKEYCODE_BUTTON_L2,     cocos2d::kCCEnumKeyCodesControllerLT},
    };

    // special controller override
    // only the main menu makes use of this difference
    if (isKeycodeControllerConfusable(keyCode) && PlatformToolbox::isControllerConnected()) {
        switch (keyCode) {
            case AKEYCODE_DPAD_DOWN:
                return cocos2d::kCCEnumKeyCodesControllerDown;
            case AKEYCODE_DPAD_UP:
                return cocos2d::kCCEnumKeyCodesControllerUp;
            case AKEYCODE_DPAD_LEFT:
                return cocos2d::kCCEnumKeyCodesControllerLeft;
            case AKEYCODE_DPAD_RIGHT:
                return cocos2d::kCCEnumKeyCodesControllerRight;
        }
    }

    if (auto it = codes.find(keyCode); it != codes.end()) {
        return it->second;
    } else {
        spdlog::get("global")->info("received unknown key code: {}", keyCode);
        return cocos2d::kCCEnumKeyCodesNone;
    }
}
