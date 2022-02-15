#include "modules/overlay.hpp"

namespace {

// i got this value by using my own device's dpi lol
constexpr float DPI_NORMAL_FACTOR = 432.0f;

bool IS_ACTIVE = false;
bool LOGGER_TRANSLUCENT = false;
float LOGGER_ALPHA = 0.9f;
bool LOGGER_AUTOSCROLL = true;

bool SCENE_TRANSLUCENT = false;
float SCENE_ALPHA = 0.9f;

bool SHOW_DEBUG_OPTIONS = false;
bool SHOW_LOGGER = false;
bool SHOW_DEMO = false;
bool SHOW_VARS = false;
bool SHOW_EXPLORER = false;

bool GAME_IN_WINDOW = false;
cocos2d::CCRenderTexture* GAME_TEX = nullptr;

float DT_COUNTER = 0.0f;
uint32_t FRAME_COUNTER = 0u;
float CURRENT_FPS = 0.0f;

void imgui_stylecolorslightgreen(ImGuiStyle* dst)
{
    // copied from https://github.com/ocornut/imgui/issues/707#issuecomment-439117182
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->WindowRounding = 3.0f; // Radius of window corners rounding. Set to 0.0f to have rectangular windows
    style->ScrollbarRounding = 3.0f; // Radius of grab corners rounding for scrollbar
    style->GrabRounding = 2.0f; // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    style->AntiAliasedLines = true;
    style->AntiAliasedFill = true;
    style->WindowRounding = 2;
    style->ChildRounding = 2;
    style->ScrollbarSize = 12;
    style->ScrollbarRounding = 3;
    style->GrabRounding = 2;
    style->ItemSpacing.x = 10;
    style->ItemSpacing.y = 6;
    style->IndentSpacing = 22;
    style->FramePadding.x = 6;
    style->FramePadding.y = 4;
    style->Alpha = 1.00f;
    style->FrameRounding = 3.0f;

    style->TouchExtraPadding = ImVec2(5.0f, 5.0f);
    style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style->PopupRounding = 3.0f;

    colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    // colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.93f, 0.93f, 0.93f, 0.98f);
    colors[ImGuiCol_Border] = ImVec4(0.71f, 0.71f, 0.71f, 0.08f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.71f, 0.71f, 0.71f, 0.55f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.94f, 0.94f, 0.94f, 0.55f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.71f, 0.78f, 0.69f, 0.98f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.82f, 0.78f, 0.78f, 0.51f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.10f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.15f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.184f, 0.407f, 0.193f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.184f, 0.407f, 0.193f, 0.78f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.184f, 0.407f, 0.193f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.71f, 0.78f, 0.69f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.725f, 0.805f, 0.702f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.793f, 0.900f, 0.836f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.71f, 0.78f, 0.69f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.71f, 0.78f, 0.69f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.71f, 0.78f, 0.69f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.45f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
}

std::string get_name_for_node(cocos2d::CCNode* node)
{
    // stolen from mat (thanks mat)
    // and also stolen from https://stackoverflow.com/a/4541470

    auto name = typeid(*node).name();
    auto status = -1;

    std::unique_ptr<char, void (*)(void*)> res {
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    };

    return (status == 0) ? res.get() : name;
}

void draw_tree_for_node(cocos2d::CCNode* node)
{
    auto ccd = cocos2d::CCDirector::sharedDirector();
    auto dpi = cocos2d::CCDevice::getDPI();

    std::ostringstream uid_ss;
    uid_ss << node->m_uID;

    if (ImGui::TreeNode(uid_ss.str().c_str(), "%s", get_name_for_node(node).c_str())) {
        ImGui::PushItemWidth(std::min(ImGui::GetWindowWidth() * 0.65f, (dpi / DPI_NORMAL_FACTOR) * 400.0f));

        {
            auto px = node->getPositionX();
            auto py = node->getPositionY();
            float pos_vals[] = { px, py };

            if (ImGui::DragFloat2("Pos.", pos_vals, 0.1f, -FLT_MAX, FLT_MAX, "%.1f")) {
                node->setPosition(cocos2d::CCPoint(pos_vals[0], pos_vals[1]));
            }
        }

        {
            auto scale = node->getScale();
            auto sx = node->getScaleX();
            auto sy = node->getScaleY();
            float scale_vals[] = { scale, sx, sy };

            if (ImGui::DragFloat3("Scale", scale_vals, 0.01f, -FLT_MAX, FLT_MAX, "%.2f")) {
                if (scale_vals[1] == scale_vals[2]) {
                    node->setScale(scale_vals[0]);
                } else {
                    node->setScaleX(scale_vals[1]);
                    node->setScaleY(scale_vals[2]);
                }
            }
        }

        {
            auto rotation = node->getRotation();
            auto rx = node->getRotationX();
            auto ry = node->getRotationY();
            float rot_vals[] = { rotation, rx, ry };

            if (ImGui::DragFloat3("Rot.", rot_vals, 0.1f, -FLT_MAX, FLT_MAX, "%.1f")) {
                if (rot_vals[1] == rot_vals[2]) {
                    node->setRotation(rot_vals[0]);
                } else {
                    node->setRotationX(rot_vals[1]);
                    node->setRotationY(rot_vals[2]);
                }
            }
        }

        {
            auto is_visible = node->isVisible();
            if (ImGui::Checkbox("Visible", &is_visible)) {
                node->setVisible(is_visible);
            }
        }

        if (dynamic_cast<cocos2d::CCRGBAProtocol*>(node) != nullptr) {
            auto c_node = dynamic_cast<cocos2d::CCRGBAProtocol*>(node);

            auto color = c_node->getColor();
            auto opacity = c_node->getOpacity();

            float color_vals[] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, opacity / 255.0f };

            if (ImGui::ColorEdit4("Color", color_vals, ImGuiColorEditFlags_Uint8)) {
                c_node->setColor({ static_cast<uint8_t>(color_vals[0] * 255),
                    static_cast<uint8_t>(color_vals[1] * 255),
                    static_cast<uint8_t>(color_vals[2] * 255) });
                c_node->setOpacity(color_vals[3] * 255);
            }
        }

        if (node->getChildrenCount() >= 1) {
            if (ImGui::TreeNode("node_children", "Children (%i)", node->getChildrenCount())) {
                auto children = node->getChildren();
                for (auto i = 0u; i < children->count(); i++) {
                    auto child = reinterpret_cast<cocos2d::CCNode*>(children->objectAtIndex(i));

                    draw_tree_for_node(child);
                }

                ImGui::TreePop();
            }
        }

        ImGui::PopItemWidth();
        ImGui::TreePop();
    }
}

void imgui_draw_cocos_window()
{
    constexpr auto window_height = 600.0f;
    constexpr auto window_width = 800.0f;
    constexpr auto window_xoffset = 120.0f;
    constexpr auto window_yoffset = 60.0f;

    auto dpi = cocos2d::CCDevice::getDPI();
    auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
    auto pixel_size = eglview->getFrameSize();

    ImGui::SetNextWindowSize(ImVec2((dpi / DPI_NORMAL_FACTOR) * window_width, (dpi / DPI_NORMAL_FACTOR) * window_height), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(window_xoffset, (pixel_size.height - window_height) - window_yoffset), ImGuiCond_FirstUseEver);

    float backup_alpha = ImGui::GetStyle().Alpha;
    if (SCENE_TRANSLUCENT) {
        ImGui::GetStyle().Alpha = SCENE_ALPHA;
    }

    if (ImGui::Begin("Scene", &SHOW_EXPLORER)) {
        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Translucent", &SCENE_TRANSLUCENT);
            if (SCENE_TRANSLUCENT) {
                ImGui::SliderFloat("Alpha", &SCENE_ALPHA, 0.25f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
            }

            ImGui::EndPopup();
        }

        if (ImGui::Button("Options")) {
            ImGui::OpenPopup("Options");
        }

        auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();

        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (scene == nullptr) {
            ImGui::Text("No running scene!");
        } else {
            ImGui::SetNextItemOpen(true);
            draw_tree_for_node(scene);
        }

        ImGui::EndChild();
    }

    if (ImGui::GetStyle().Alpha != backup_alpha) {
        ImGui::GetStyle().Alpha = backup_alpha;
    }

    ImGui::End();
}

void imgui_draw_log_window()
{
    constexpr auto window_height = 600.0f;
    constexpr auto window_width = 800.0f;
    constexpr auto window_xoffset = 60.0f;
    constexpr auto window_yoffset = 60.0f;

    auto dpi = cocos2d::CCDevice::getDPI();
    auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
    auto pixel_size = eglview->getFrameSize();

    ImGui::SetNextWindowSize(ImVec2((dpi / DPI_NORMAL_FACTOR) * window_width, (dpi / DPI_NORMAL_FACTOR) * window_height), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(window_xoffset, (pixel_size.height - window_height) - window_yoffset), ImGuiCond_FirstUseEver);

    // push/pop style crashed for... whatever reason
    float backup_alpha = ImGui::GetStyle().Alpha;
    if (LOGGER_TRANSLUCENT) {
        ImGui::GetStyle().Alpha = LOGGER_ALPHA;
    }

    if (ImGui::Begin("Logs", &SHOW_LOGGER)) {
        if (ImGui::BeginPopup("Options")) {
            ImGui::Checkbox("Translucent", &LOGGER_TRANSLUCENT);
            if (LOGGER_TRANSLUCENT) {
                ImGui::SliderFloat("Alpha", &LOGGER_ALPHA, 0.25f, 1.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
            }

            ImGui::Checkbox("Auto-scroll", &LOGGER_AUTOSCROLL);
            ImGui::EndPopup();
        }

        if (ImGui::Button("Options")) {
            ImGui::OpenPopup("Options");
        }

        ImGui::Separator();

        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, 0);

        auto ring_buffer = std::static_pointer_cast<spdlog::sinks::ringbuffer_sink_mt>(spdlog::get("global")->sinks().front());
        for (const auto& line : ring_buffer->last_formatted()) {
            ImGui::TextWrapped("%s", line.c_str());
        }

        if (LOGGER_AUTOSCROLL && (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
    }

    if (ImGui::GetStyle().Alpha != backup_alpha) {
        ImGui::GetStyle().Alpha = backup_alpha;
    }

    ImGui::End();
}

void imgui_draw_vars_window()
{
    if (ImGui::Begin("Variables", &SHOW_VARS, ImGuiWindowFlags_NoResize)) {
        auto gm = GameManager::sharedState();

        auto nc_val = gm->getGameVariable("4099");
        if (ImGui::Checkbox("Ignore Damage", &nc_val)) {
            gm->toggleGameVariable("4099");
        }

        auto pm_val = gm->getGameVariable("4098");
        if (ImGui::Checkbox("Practice Music", &pm_val)) {
            gm->toggleGameVariable("4098");
        }

        auto sc_val = gm->getGameVariable("4093");
        if (ImGui::Checkbox("Speedhack Controls", &sc_val)) {
            gm->toggleGameVariable("4093");
        }

        auto ru_val = gm->getGameVariable("4094");
        if (ImGui::Checkbox("Replay Utils", &ru_val)) {
            gm->toggleGameVariable("4094");
        }

        auto vfb_val = gm->getGameVariable("3001");
        if (ImGui::Checkbox("Bypass Verify", &vfb_val)) {
            gm->toggleGameVariable("3001");
        }
    }

    ImGui::End();
}

void imgui_draw_overlay_options()
{
    auto title_string = cocos2d::CCString::createWithFormat("1.9 GDPS u%i.%i.%i",
        CMakeConfiguration::VER_MAJOR, CMakeConfiguration::VER_MINOR,
        CMakeConfiguration::VER_PATCH);

    ImGui::Begin(title_string->getCString(), &SHOW_DEBUG_OPTIONS, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Windows")) {
            ImGui::MenuItem("Logs", nullptr, &SHOW_LOGGER);
            ImGui::MenuItem("Variables", nullptr, &SHOW_VARS);
            ImGui::MenuItem("Scene", nullptr, &SHOW_EXPLORER);
            ImGui::Separator();
            ImGui::MenuItem("Framebuffer", nullptr, &GAME_IN_WINDOW);
            ImGui::MenuItem("Demo", nullptr, &SHOW_DEMO);

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Shortcuts")) {
            if (ImGui::MenuItem("Open MoreOptionsLayer")) {
                MoreOptionsLayer::create()->show();
            }

            if (ImGui::MenuItem("Open SettingsPopup")) {
                Settings::SettingsPopup::create()->show();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Clear Save Data")) {
                spdlog::get("global")->warn("clearing all save data");

                GameManager::sharedState()->firstLoad();
                GJAccountManager::sharedState()->unlinkFromAccount();

                cocos2d::CCDirector::sharedDirector()->replaceScene(
                    MenuLayer::scene(false));
            }

            if (ImGui::MenuItem("Crash Game")) {
                spdlog::get("global")->warn("manual crash called");

                reinterpret_cast<cocos2d::CCDirector*>(0x0)->init();
            }

            if (ImGui::MenuItem("Save Game")) {
                auto file = cocos2d::CCFileUtils::sharedFileUtils();

                spdlog::get("global")->info("calling for game save to path {}", file->getAndroidPath());

                AppDelegate::get()->trySaveGame();
            }

            if (ImGui::MenuItem("Reload All")) {
                GameManager::sharedState()->reloadAll(false, false, false);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Text("FPS: %.1f", CURRENT_FPS);
    ImGui::Text("%s-%s\n%s", CMakeConfiguration::BRANCH, CMakeConfiguration::HASH, CMakeConfiguration::COMPILE_TIME);
    ImGui::Text("Development Build");

    ImGui::End();
}

void imgui_start_frame()
{
    if (!IS_ACTIVE) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();

    auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
    auto pixel_size = eglview->getFrameSize();

    io.DisplaySize = ImVec2(pixel_size.width, pixel_size.height);
    if (pixel_size.width > 0.0f && pixel_size.height > 0.0f) {
        // is this taking advantage of float rounding?? copied from ImGui_ImplAndroid_NewFrame
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    }

    ImGui::NewFrame();
}

void imgui_tick(void* tex = nullptr)
{
    if (!IS_ACTIVE) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();

    /*
        // todo: use cocos methods for this (if needed)
    static bool WantTextInputLast = false;
    if (io.WantTextInput && !WantTextInputLast)
        ShowSoftKeyboardInput();
    WantTextInputLast = io.WantTextInput;
*/

    if (SHOW_DEBUG_OPTIONS) {
        imgui_draw_overlay_options();
    }

    if (SHOW_LOGGER) {
        imgui_draw_log_window();
    }

    if (SHOW_DEMO) {
        ImGui::ShowDemoWindow(&SHOW_DEMO);
    }

    if (SHOW_VARS) {
        imgui_draw_vars_window();
    }

    if (SHOW_EXPLORER) {
        imgui_draw_cocos_window();
    }

    if (GAME_IN_WINDOW && tex) {
        if (ImGui::Begin("Geometry Dash", &GAME_IN_WINDOW)) {
            ImGui::BeginChild("buffer");

            auto wsize = ImGui::GetWindowSize();
            auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
            auto pixel_size = eglview->getFrameSize();

            auto window_ratio = pixel_size.width / pixel_size.height;

            if (wsize.x < wsize.y) {
                wsize.y = wsize.x / window_ratio;
            } else {
                wsize.x = window_ratio * wsize.y;
            }

            {
                // center image in window (cause why not)
                auto c_size = ImGui::GetWindowSize();
                ImGui::SetCursorPos(ImVec2((c_size.x - wsize.x) * 0.5f, (c_size.y - wsize.y) * 0.5f));
            }

            ImGui::Image(reinterpret_cast<ImTextureID>(tex), wsize, ImVec2(0, 0.297), ImVec2(0.297, 0));
            ImGui::EndChild();
        }

        ImGui::End();
    }

    auto window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground;
    auto height = ImGui::GetFrameHeight();

    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.5f, 0.5f, 0.5f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    if (ImGui::BeginViewportSideBar("##MainStatusBar", nullptr, ImGuiDir_Down, height, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::Text("DEV %s-%s", CMakeConfiguration::BRANCH, CMakeConfiguration::HASH);
            ImGui::Text("FPS: %.1f", CURRENT_FPS);

            ImGui::MenuItem("More", nullptr, &SHOW_DEBUG_OPTIONS);
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();

    ImGui::PopStyleColor(1);
}

void imgui_end_frame()
{
    if (!IS_ACTIVE) {
        return;
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_init()
{
    spdlog::get("global")->info("imgui initializing");

    if (IS_ACTIVE) {
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.BackendPlatformName = "imgui_impl_cocos2dx";
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

    // official example recommends using <Load/Save>IniSettingsToMemory but i can't be bothered rn
    io.IniFilename = nullptr;

    //    ImGui_ImplAndroid_Init(window);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    ImGui::UpdateHoveredWindowAndCaptureFlags();

    imgui_stylecolorslightgreen(nullptr);

    // default font with increased size. just like the example (wow)
    auto dpi = cocos2d::CCDevice::getDPI();
    auto default_font_size = std::floor((dpi / DPI_NORMAL_FACTOR) * 32.0f);

    unsigned long size = 0;
    auto full_path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("fonts/FiraMono-Regular.ttf", false);
    auto font_buffer = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(full_path.c_str(), "rb", &size);

    spdlog::get("global")->info("loading font from {} (sz {})", full_path, size);

    if (font_buffer == nullptr || size <= 0) {
        ImFontConfig font_cfg;
        font_cfg.SizePixels = default_font_size;
        io.Fonts->AddFontDefault(&font_cfg);
    } else {
        io.Fonts->AddFontFromMemoryTTF(font_buffer, size, default_font_size);
        //        delete[] font_buffer; // leaving this open causes memory dealloc crashes
    }

    ImGui::GetStyle().ScaleAllSizes((dpi / DPI_NORMAL_FACTOR) * 3.5f);

    IS_ACTIVE = true;
}

void imgui_close()
{
    spdlog::get("global")->info("imgui close called");

    if (!IS_ACTIVE) {
        return;
    }

    ImGui_ImplOpenGL3_Shutdown();
    //    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();

    IS_ACTIVE = false;
}

void CCDirector_drawScene(cocos2d::CCDirector* self)
{
    imgui_start_frame();

    // if changes mid run
    bool show_in_window = GAME_IN_WINDOW;

    void* tex = nullptr;

    if (GAME_IN_WINDOW) {
        if (!GAME_TEX) {
            auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
            auto pixel_size = eglview->getFrameSize();

            GAME_TEX = cocos2d::CCRenderTexture::create(pixel_size.width, pixel_size.height, cocos2d::kCCTexture2DPixelFormat_RGBA4444);
            GAME_TEX->retain();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GAME_TEX->begin();
    }

    HookHandler::orig<&CCDirector_drawScene>(self);

    if (GAME_IN_WINDOW) {
        GAME_TEX->end();
        tex = reinterpret_cast<void*>(GAME_TEX->getSprite()->getTexture()->getName());
    }

    if (IS_ACTIVE) {
        imgui_tick(tex);
        imgui_end_frame();

        glFlush();
    }
}

int CCApplication_run(cocos2d::CCApplication* self)
{
    auto r = HookHandler::orig<&CCApplication_run>(self);

    // if issues arise we may need to add global variable
    imgui_init();

    return r;
}

void CCEGLView_end(cocos2d::CCEGLView* self)
{
    // something causes a crash, somewhere. doesn't seem to break data though
    if (IS_ACTIVE) {
        imgui_close();
    }

    // todo: figure out why this causes big crash
    terminateProcessJNI();
}

void CCTouchDispatcher_touches(cocos2d::CCTouchDispatcher* self, cocos2d::CCSet* touches, cocos2d::CCEvent* event, uint32_t index)
{
    // note that touch support is still very buggy. like incredibly buggy
    // hopefully in the future it can be stabilized
    if (!IS_ACTIVE) {
        HookHandler::orig<&CCTouchDispatcher_touches>(self, touches, event, index);
        return;
    }

    ImGuiIO& io = ImGui::GetIO();

    // this prevents "hovering" from beginning capture
    bool pass_mouse_pos_to_gui = io.WantCaptureMouse;

    switch (index) {
    case 0: { // CCTOUCHBEGAN
        auto touch = reinterpret_cast<cocos2d::CCTouch*>(touches->anyObject());
        if (touch != nullptr) {
            auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
            auto loc = touch->getLocationInView();

            io.AddMousePosEvent(loc.x * eglview->getScaleX(), loc.y * eglview->getScaleY());
        }

        io.AddMouseButtonEvent(0, true);
        break;
    }
    case 1: { // CCTOUCHMOVED
        auto touch = reinterpret_cast<cocos2d::CCTouch*>(touches->anyObject());
        if (touch != nullptr && pass_mouse_pos_to_gui) {
            auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
            auto loc = touch->getLocationInView();

            io.AddMousePosEvent(loc.x * eglview->getScaleX(), loc.y * eglview->getScaleY());
        }
        break;
    }
    case 2: // CCTOUCHENDED
    case 3: // CCTOUCHCANCELLED
        io.AddMouseButtonEvent(0, false);
        break;
    }

    // always pass end/cancel events. may introduce crashes but should fix other bugs
    if (!io.WantCaptureMouse || index == 2 || index == 3) {
        HookHandler::orig<&CCTouchDispatcher_touches>(self, touches, event, index);
    }
}

void CCScheduler_update(cocos2d::CCScheduler* self, float dt)
{
    FRAME_COUNTER++;
    DT_COUNTER += dt;

    if (DT_COUNTER >= 0.5f) {
        CURRENT_FPS = FRAME_COUNTER / DT_COUNTER;

        FRAME_COUNTER = 0u;
        DT_COUNTER = 0.0f;
    }

    return HookHandler::orig<&CCScheduler_update>(self, dt);
}
}

namespace Overlay {
void Module::on_initialize()
{
    HookHandler::get_handler()
        .add_hook("_ZN7cocos2d13CCApplication3runEv", &CCApplication_run)
        .add_hook("_ZN7cocos2d10CCDirector9drawSceneEv", &CCDirector_drawScene)
        .add_hook("_ZN7cocos2d17CCTouchDispatcher7touchesEPNS_5CCSetEPNS_7CCEventEj", &CCTouchDispatcher_touches)
        .add_hook("_ZN7cocos2d9CCEGLView3endEv", &CCEGLView_end)
        .add_hook<1>("_ZN7cocos2d11CCScheduler6updateEf", &CCScheduler_update);
}
}