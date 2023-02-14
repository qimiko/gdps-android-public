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

    void PlayLayer_resume(PlayLayer* self) {
        spdlog::info("PlayLayer::resume called");

        HookHandler::orig<&PlayLayer_resume>(self);

        if (AppDelegate::get()->getPaused()) {
            return;
        }

        if (!GameManager::sharedState()->getGameVariable(GameVariable::SHOW_CURSOR)) {
            if (PlatformToolbox::isControllerConnected()) {
                ControllerManager::getManager().hideCursor();
            }
        }
    }

    void PlayLayer_resumeAndRestart(PlayLayer* self) {
        spdlog::info("PlayLayer::resumeAndRestart called");

        HookHandler::orig<&PlayLayer_resumeAndRestart>(self);

        if (AppDelegate::get()->getPaused()) {
            return;
        }

        if (!GameManager::sharedState()->getGameVariable(GameVariable::SHOW_CURSOR)) {
            if (PlatformToolbox::isControllerConnected()) {
                ControllerManager::getManager().hideCursor();
            }
        }
    }

    void EndLevelLayer_onReplay(EndLevelLayer* self, cocos2d::CCObject* target) {
        spdlog::info("EndLevelLayer::onReplay called");

        HookHandler::orig<&EndLevelLayer_onReplay>(self, target);

        if (!GameManager::sharedState()->getGameVariable(GameVariable::SHOW_CURSOR)) {
            if (PlatformToolbox::isControllerConnected()) {
                ControllerManager::getManager().hideCursor();
            }
        }
    }

    void RetryLevelLayer_onReplay(RetryLevelLayer* self, cocos2d::CCObject* target) {
        spdlog::info("RetryLevelLayer::onReplay called");

        HookHandler::orig<&RetryLevelLayer_onReplay>(self, target);

        if (!GameManager::sharedState()->getGameVariable(GameVariable::SHOW_CURSOR)) {
            if (PlatformToolbox::isControllerConnected()) {
                ControllerManager::getManager().hideCursor();
            }
        }
    }

    void PlayLayer_showRetryLayer(PlayLayer* self) {
        spdlog::info("PlayLayer::showRetryLayer called");

        HookHandler::orig<&PlayLayer_showRetryLayer>(self);

        if (PlatformToolbox::isControllerConnected()) {
            ControllerManager::getManager().showCursor();
        }
    }

    void PlayLayer_showEndLayer(PlayLayer* self) {
        spdlog::info("PlayLayer::showEndLayer called");

        HookHandler::orig<&PlayLayer_showEndLayer>(self);

        if (PlatformToolbox::isControllerConnected()) {
            ControllerManager::getManager().showCursor();
        }
    }

    void PlayLayer_onQuit(PlayLayer* self) {
        spdlog::info("PlayLayer::onQuit called");

        HookHandler::orig<&PlayLayer_onQuit>(self);

        if (PlatformToolbox::isControllerConnected()) {
            ControllerManager::getManager().showCursor();
        }
    }

    void PlayLayer_pauseGame(PlayLayer* self, bool p1) {
        spdlog::info("PlayLayer::pauseGame({}) called", p1);

        HookHandler::orig<&PlayLayer_pauseGame>(self, p1);

        // locked to !PlayLayer::getShowingEndLayer() && !PlayLayer::getEndTriggered()


        if (!AppDelegate::get()->getPaused()) {
            return;
        }

        if (PlatformToolbox::isControllerConnected()) {
            ControllerManager::getManager().showCursor();
        }
    }

    void PlayLayer_destructor(PlayLayer* self) {
        spdlog::info("PlayLayer::~PlayLayer called");

        HookHandler::orig<&PlayLayer_destructor>(self);

        if (PlatformToolbox::isControllerConnected()) {
            ControllerManager::getManager().showCursor();
        }
    }

    void AppDelegate_applicationDidEnterBackground(AppDelegate* self) {
        // game may deadlock at this line...
        FMODAudioEngine::sharedEngine()->stop();
        FMODAudioEngine::sharedEngine()->update(0.0f);

        self->pauseGame();
        self->trySaveGame();

        cocos2d::CCDirector::sharedDirector()->stopAnimation();
        cocos2d::CCDirector::sharedDirector()->pause();

        FMODAudioEngine::sharedEngine()->pauseAllEffects();

        if (self->getLoadingFinished()) {
            FMODAudioEngine::sharedEngine()->pauseBackgroundMusic();
            PlatformToolbox::onNativePause();
            GameManager::sharedState()->applicationDidEnterBackground();
        }

        FMODAudioEngine::sharedEngine()->update(0.0f);
    }

    void AppDelegate_applicationWillEnterForeground(AppDelegate* self) {
        cocos2d::CCDirector::sharedDirector()->stopAnimation();
        cocos2d::CCDirector::sharedDirector()->resume();
        cocos2d::CCDirector::sharedDirector()->startAnimation();

        FMODAudioEngine::sharedEngine()->start();
        FMODAudioEngine::sharedEngine()->update(0.0f);

        if (PlatformToolbox::shouldResumeSound()) {
            self->resumeSound();
        }

        PlatformToolbox::onNativeResume();
        PlatformToolbox::onToggleKeyboard();

        cocos2d::CCDirector::sharedDirector()->getActionManager()
            ->removeActionByTag(1, GameManager::sharedState());

        auto application_enter_sequence = cocos2d::CCSequence::create(
                cocos2d::CCDelayTime::create(0.0f),
                cocos2d::CCCallFunc::create(
                        GameManager::sharedState(),
                        static_cast<cocos2d::SEL_CallFunc>(&GameManager::applicationWillEnterForeground)
                )
        );

        application_enter_sequence->setTag(1);

        cocos2d::CCDirector::sharedDirector()->getActionManager()
                ->addAction(
                        application_enter_sequence,
                        GameManager::sharedState(),
                        false
                        );

        FMODAudioEngine::sharedEngine()->update(0.0f);
    }
}

namespace Input {
    void Module::on_initialize()
    {
        HookHandler::get_handler()
                .add_hook("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown", &Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown)
                .add_hook(&PlayLayer::resume, &PlayLayer_resume)
                .add_hook(&PlayLayer::resumeAndRestart, &PlayLayer_resumeAndRestart)
                .add_hook(&EndLevelLayer::onReplay, &EndLevelLayer_onReplay)
                .add_hook(&RetryLevelLayer::onReplay, &RetryLevelLayer_onReplay)
                .add_hook(&PlayLayer::showRetryLayer, &PlayLayer_showRetryLayer)
                .add_hook(&PlayLayer::showEndLayer, &PlayLayer_showEndLayer)
                .add_hook(&PlayLayer::onQuit, &PlayLayer_onQuit)
                .add_hook(&PlayLayer::pauseGame, &PlayLayer_pauseGame)
                .add_hook("_ZN9PlayLayerD0Ev", &PlayLayer_destructor)
//                .add_hook("_ZN11AppDelegate29applicationDidEnterBackgroundEv", &AppDelegate_applicationDidEnterBackground)
                .add_hook(PlatformToolbox::isControllerConnected, &PlatformToolbox_isControllerConnected);
    }
} // namespace Input
