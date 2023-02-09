#include "modules/input.hpp"

namespace {
    extern "C" jboolean Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown(JNIEnv *env, jclass thiz, int keyCode) {
        if (keyCode == AKEYCODE_BUTTON_THUMBL) {
            ControllerManager::getManager().handleReset();
            return true;
        }

        if (keyCode == AKEYCODE_BUTTON_A) {
            ControllerManager::getManager().handleClickDown();
            return true;
        }

        if (keyCode != 0x4 && keyCode != 0x52) {
            auto keyboard_dispatcher = cocos2d::CCDirector::sharedDirector()->getKeyboardDispatcher();
            auto translated_code = ControllerManager::translateAndroidKeyCodeToWindows(keyCode);

            if (translated_code == cocos2d::kCCEnumKeyCodesControl) {
                keyboard_dispatcher->updateModifierKeys(
                        keyboard_dispatcher->getShiftKeyPressed(),
                        true,
                        keyboard_dispatcher->getAltKeyPressed(),
                        false);
            }

            if (translated_code == cocos2d::kCCEnumKeyCodesShift) {
                keyboard_dispatcher->updateModifierKeys(
                        true,
                        keyboard_dispatcher->getControlKeyPressed(),
                        keyboard_dispatcher->getAltKeyPressed(),
                        false);
            }

            if (translated_code == cocos2d::kCCEnumKeyCodesAlt) {
                keyboard_dispatcher->updateModifierKeys(
                        keyboard_dispatcher->getShiftKeyPressed(),
                        keyboard_dispatcher->getControlKeyPressed(),
                        true,
                        false);
            }

            return keyboard_dispatcher->dispatchKeyboardMSG(translated_code, true);
        } else {
            return HookHandler::orig<&Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown>(env, thiz, keyCode);
        }
    }

    bool PlatformToolbox_isControllerConnected() {
        return JNI::is_controller_connected();
    }
}

namespace Input {
    void Module::on_initialize()
    {
        HookHandler::get_handler()
                .add_hook("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown", &Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown)
                .add_hook(PlatformToolbox::isControllerConnected, &PlatformToolbox_isControllerConnected);
    }
} // namespace Input
