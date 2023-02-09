#include "classes/callbacks/inputcallback.hpp"

extern "C" {
[[gnu::visibility("default")]] jboolean Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyUp(JNIEnv * /* env */, jclass /* clazz */, int keyCode)
{
    if (keyCode == AKEYCODE_BUTTON_A) {
        ControllerManager::getManager().handleClickUp();
        return true;
    }

    if (keyCode != 0x4 && keyCode != 0x52) {
        auto keyboard_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeyboardDispatcher();
        auto translated_code = ControllerManager::translateAndroidKeyCodeToWindows(keyCode);

        if (translated_code == cocos2d::kCCEnumKeyCodesControl) {
            keyboard_dispatcher->updateModifierKeys(
                    keyboard_dispatcher->getShiftKeyPressed(),
                    false,
                    keyboard_dispatcher->getAltKeyPressed(),
                    false);
        }

        if (translated_code == cocos2d::kCCEnumKeyCodesShift) {
            keyboard_dispatcher->updateModifierKeys(
                    false,
                    keyboard_dispatcher->getControlKeyPressed(),
                    keyboard_dispatcher->getAltKeyPressed(),
                    false);
        }

        if (translated_code == cocos2d::kCCEnumKeyCodesAlt) {
            keyboard_dispatcher->updateModifierKeys(
                    keyboard_dispatcher->getShiftKeyPressed(),
                    keyboard_dispatcher->getControlKeyPressed(),
                    false,
                    false);
        }

        return keyboard_dispatcher->dispatchKeyboardMSG(translated_code, false);
    }

    return true;
}

[[gnu::visibility("default")]] jboolean Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeActionScroll(JNIEnv * /* env */, jclass /* clazz */, float scrollX, float scrollY)
{
    spdlog::get("global")->trace("scroll params: {} {}", scrollX, scrollY);

    // android scroll values: [-1, 1], -1 is down on the scroll
    // windows scroll values: [-120, 120], 120 is down on the scroll
    // but rob uses glfw thanks robtop, no idea what changed with the values but they different

    constexpr auto SCROLL_FACTOR = -15.0f;

    return cocos2d::CCDirector::sharedDirector()->getMouseDispatcher()->dispatchScrollMSG(
            scrollX * SCROLL_FACTOR, scrollY * SCROLL_FACTOR);
}

[[gnu::visibility("default")]] jboolean Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeActionJoystickMove(JNIEnv * /* env */, jclass /* clazz */, float leftX, float leftY, float rightX, float rightY) {
    return ControllerManager::getManager().updateJoystickValues({ leftX, leftY }, { rightX, rightY });
}
}