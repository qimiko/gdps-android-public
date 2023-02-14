#pragma once
#ifndef JNI_FUNCS_H
#define JNI_FUNCS_H

#include <jni/JniHelper.h>

namespace JNI {
    bool is_gd_installed();
    void export_crash_dump(const std::string& path);
    void keep_screen_awake();
    void remove_screen_awake();
    void open_texture_picker();
    void apply_classic_pack();
    void wipe_textures_directory();
    void export_level(const std::string& name);
    void import_level();
    void loaded_to_menu();
    std::string get_textures_directory();
    bool is_controller_connected();
    std::string get_secondary_assets_directory();
    std::string get_save_directory();
    bool is_launcher_build();
    bool is_screen_restricted();
    void toggle_is_screen_restricted();
    void capture_cursor();
    void release_cursor();
}

#endif