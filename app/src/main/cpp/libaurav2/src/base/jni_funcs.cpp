#include "base/jni_funcs.hpp"

bool JNI::is_gd_installed()
{
    cocos2d::JniMethodInfo t;
    auto ret = false;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "isGeometryDashInstalled", "()Z"))
    {
        ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
    }

    return ret;
}

void JNI::export_crash_dump(const std::string& path) {
    if (path.empty()) {
        return;
    }

    cocos2d::JniMethodInfo t;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "exportCrashDump", "(Ljava/lang/String;)V"))
    {
        auto jpath = t.env->NewStringUTF(path.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jpath);
    }
}

void JNI::keep_screen_awake() {
    cocos2d::JniMethodInfo t;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "keepScreenAwake", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }
}

void JNI::remove_screen_awake() {
    cocos2d::JniMethodInfo t;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "removeScreenAwake", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }
}

void JNI::open_texture_picker() {
    cocos2d::JniMethodInfo t;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "showTexturePicker", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }
}

void JNI::apply_classic_pack() {
    cocos2d::JniMethodInfo t;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "applyClassicPack", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }
}

void JNI::wipe_textures_directory() {
    cocos2d::JniMethodInfo t;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "wipeTexturesDirectory", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }
}

void JNI::export_level(const std::string& name) {
    if (name.empty()) {
        return;
    }

    cocos2d::JniMethodInfo t;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "onExportLevel", "(Ljava/lang/String;)V"))
    {
        auto jname = t.env->NewStringUTF(name.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jname);
    }
}

void JNI::import_level() {
    cocos2d::JniMethodInfo t;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "showLevelPicker", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }
}

void JNI::loaded_to_menu() {
    cocos2d::JniMethodInfo t;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "loadedToMenu", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }
}

std::string JNI::get_textures_directory() {
    cocos2d::JniMethodInfo t;
    std::string ret("");

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "getTexturesDirectory", "()Ljava/lang/String;"))
    {
        auto str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
        t.env->DeleteLocalRef(t.classID);
        ret = cocos2d::JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }

    return ret;
}

bool JNI::is_controller_connected() {
    cocos2d::JniMethodInfo t;
    auto ret = false;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "isControllerConnected", "()Z"))
    {
        ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
    }

    return ret;
}

std::string JNI::get_secondary_assets_directory() {
    cocos2d::JniMethodInfo t;
    std::string ret("");

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "getSecondaryAssetsDirectory", "()Ljava/lang/String;"))
    {
        auto str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
        t.env->DeleteLocalRef(t.classID);
        ret = cocos2d::JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }

    return ret;
}

std::string JNI::get_save_directory() {
    cocos2d::JniMethodInfo t;
    std::string ret("");

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "getSaveDirectory", "()Ljava/lang/String;"))
    {
        auto str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
        t.env->DeleteLocalRef(t.classID);
        ret = cocos2d::JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }

    return ret;
}

bool JNI::is_launcher_build() {
    cocos2d::JniMethodInfo t;
    auto ret = false;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "isLauncherBuild", "()Z"))
    {
        ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
    }

    return ret;
}

bool JNI::is_screen_restricted() {
    cocos2d::JniMethodInfo t;
    auto ret = false;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "isScreenRestricted", "()Z"))
    {
        ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
    }

    return ret;
}

void JNI::toggle_is_screen_restricted() {
    cocos2d::JniMethodInfo t;

    if (cocos2d::JniHelper::getStaticMethodInfo(t, "com/kyurime/geometryjump/ModGlue", "toggleIsScreenRestricted", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
    }
}