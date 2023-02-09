#include "classes/loadingfailedhijacklayer.hpp"

bool LoadingFailedHijackLayer::gHasDestroyedState = false;

bool LoadingFailedHijackLayer::hasDestroyedState() {
    return gHasDestroyedState;
}

void LoadingFailedHijackLayer::FLAlert_Clicked(FLAlertLayer*, bool) {
// currently clicked button is irrelevant
    restoreGameState();

    cocos2d::CCDirector::sharedDirector()->replaceScene(LoadingLayer::scene(false));
};

bool LoadingFailedHijackLayer::init() {
    if (CCLayer::init()) {
        auto win_size = cocos2d::CCDirector::sharedDirector()->getWinSize();

        auto bg_filename = GameManager::sharedState()->getBGTexture(1);
        auto bg_sprite = cocos2d::CCSprite::create(bg_filename);

        this->addChild(bg_sprite);
        bg_sprite->setPosition(win_size / 2);
        bg_sprite->setScale(AppDelegate::get()->bgScale());
        bg_sprite->setColor({ 0, 102, 255 });

        return true;
    }

    return false;
}

void LoadingFailedHijackLayer::showLoadFailedDialog() {
    FLAlertLayer::create(
            this,
            "Load Failed",
            "An <co>unknown problem</c> has been found with your <cl>save data</c>. "
            "The game will <cy>not continue</c> unless you <cr>reset your progress</c>.",
            "Reset :(",
            nullptr,
            350.0f
    )->show();
}

void LoadingFailedHijackLayer::onEnterDelayFinished(cocos2d::CCNode*) {
    showLoadFailedDialog();
}

void LoadingFailedHijackLayer::onEnter() {
    CCLayer::onEnter();

    // make it look like we actually did something, lol
    auto cc_func = cocos2d::CCCallFuncN::create(
            this,
            static_cast<cocos2d::SEL_CallFuncN>(&LoadingFailedHijackLayer::onEnterDelayFinished)
    );

    auto load_action = cocos2d::CCSequence::createWithTwoActions(
            cocos2d::CCDelayTime::create(0.5f),
            cc_func
    );

    this->runAction(load_action);
}

void LoadingFailedHijackLayer::destroyGameState() {
    // patch out scene replacement/save
    if (gHasDestroyedState) {
        return;
    }

    // this patch destroys loadAsset for testing
    // HookHandler::get_handler().install_patch(0x1AF2C6, { 0xF0, 0xBD });

    spdlog::get("global")->warn("destroying game state!");

    HookHandler::get_handler()
//            .install_patch(0x265504, { 0x10, 0xBD }) // patches out runWithScene
//            .install_patch(0x1AC166, { 0xFE, 0xBD }) // patches out remaining applicationDidFinishLaunching
            .install_patch(0x29CB96, { 0x70, 0xBD }); // patches out saveRootSubDictToCompressedFile

    gHasDestroyedState = true;
}

void LoadingFailedHijackLayer::restoreGameState() {
    // unpatch scene replacement/save
    // we will probably need to reload the loading scene from here

    if (!gHasDestroyedState) {
        return;
    }

    HookHandler::get_handler()
//                .install_patch(0x265504, { 0x04, 0x46 }) // restores runWithScene
//                .install_patch(0x1AC166, { 0x03, 0xF0 }) // restores remaining applicationDidFinishLaunching
            .install_patch(0x29CB96, { 0xAD, 0xF5 }); // restores DS_Dictionary save

    gHasDestroyedState = false;
}

cocos2d::CCScene* LoadingFailedHijackLayer::scene() {
    auto scene = cocos2d::CCScene::create();
    scene->addChild(LoadingFailedHijackLayer::create());

    return scene;
}