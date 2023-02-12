#include "modules/overlay.hpp"

namespace {

// i got this value by using my own device's dpi lol
constexpr float DPI_NORMAL_FACTOR = 432.0f;

bool IS_ACTIVE = false;
bool LOGGER_TRANSLUCENT = false;
float LOGGER_ALPHA = 0.9f;
bool LOGGER_AUTOSCROLL = true;
bool LOGGER_SHOW_DEBUG = true;

bool SCENE_TRANSLUCENT = false;
float SCENE_ALPHA = 0.9f;

bool SHOW_DEBUG_OPTIONS = false;
bool SHOW_LOGGER = false;
bool SHOW_LOG_OVERLAY = true;
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
    ImVec4 *colors = style->Colors; // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)

    style->WindowRounding = 6.0f; // Radius of window corners rounding. Set to 0.0f to have rectangular windows
    style->ScrollbarRounding = 3.0f; // Radius of grab corners rounding for scrollbar
    style->GrabRounding = 3.0f; // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    style->PopupRounding = 6.0f;

    style->AntiAliasedLines = true;
    style->AntiAliasedFill = true;
    style->ChildRounding = 2;
    style->ScrollbarSize = 12;
    style->ItemSpacing.x = 10;
    style->ItemSpacing.y = 6;
    style->IndentSpacing = 22;
    style->FramePadding.x = 6;
    style->FramePadding.y = 4;
    style->Alpha = 1.00f;
    style->FrameRounding = 3.0f;

    style->WindowBorderSize = 1.0f;
    style->PopupBorderSize = 1.0f;

    style->TouchExtraPadding = ImVec2(5.0f, 5.0f);
    style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

    colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    // colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.25f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.04f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 0.55f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.94f, 0.94f, 0.94f, 0.55f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.71f, 0.78f, 0.69f, 0.98f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.10f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.15f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_CheckMark] = ImVec4(0.184f, 0.407f, 0.193f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_SliderGrab] = ImVec4(0.184f, 0.407f, 0.193f, 0.78f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.184f, 0.407f, 0.193f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_Button] = ImVec4(0.71f, 0.78f, 0.69f, 0.40f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.725f, 0.805f, 0.702f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_ButtonActive] = ImVec4(0.793f, 0.900f, 0.836f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_Header] = ImVec4(0.71f, 0.78f, 0.69f, 0.31f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.71f, 0.78f, 0.69f, 0.80f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_HeaderActive] = ImVec4(0.71f, 0.78f, 0.69f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.725f, 0.805f, 0.702f, 0.70f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.793f, 0.900f, 0.836f, 0.80f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
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

            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
            if (ImGui::DragFloat2("Pos.", pos_vals, 0.1f, -FLT_MAX, FLT_MAX, "%.1f")) {
                node->setPosition(cocos2d::CCPoint(pos_vals[0], pos_vals[1]));
            }
        }

        {
            auto scale = node->getScale();
            auto sx = node->getScaleX();
            auto sy = node->getScaleY();
            float scale_vals[] = { scale, sx, sy };

            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
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

            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
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

            float color_vals[] = {
                static_cast<float>(color.r) / 255.0f,
                static_cast<float>(color.g) / 255.0f,
                static_cast<float>(color.b) / 255.0f,
                static_cast<float>(opacity) / 255.0f };

            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
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

ImVec4 get_log_color(spdlog::level::level_enum level, bool useDark = false) {
    switch (level) {
        case spdlog::level::trace:
        case spdlog::level::debug:
            return useDark ? ImVec4(0.8f, 0.8f, 0.8f, 1.0f) : ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        case spdlog::level::info:
            return useDark ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        case spdlog::level::warn:
            // 255, 221, 87 or 193, 131, 1, 255
            return useDark ? ImVec4(1.0f, 0.867f, 0.341f, 1.0f) : ImVec4(0.757f, 0.514f, 0.004f, 1.0f);
        case spdlog::level::err:
            // 255, 52, 33 or 228, 86, 73, 255
            return useDark ? ImVec4(1.0f, 0.204f, 0.129f, 1.0f) : ImVec4(0.894f, 0.337f, 0.286f, 1.0f);
        default:
            return { 0.0f, 0.0f, 0.0f, 1.0f };
    }
}

void imgui_draw_logs(bool useDark = false) {
    auto ring_buffer = std::static_pointer_cast<spdlog::sinks::ringbuffer_sink_mt>(spdlog::get("global")->sinks().front());
    for (const auto& line : ring_buffer->last_raw()) {
        if (!LOGGER_SHOW_DEBUG && (line.level == spdlog::level::debug || line.level == spdlog::level::trace)) {
            continue;
        }

        ImGui::PushStyleColor(ImGuiCol_Text, get_log_color(line.level, useDark));

        auto log_details = fmt::format(
                "[{:%H:%M:%S}] [{}] {}",
                line.time, to_short_c_str(line.level), line.payload);
        ImGui::TextWrapped("%s", log_details.c_str());

        ImGui::PopStyleColor();
    }
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
            ImGui::Checkbox("Show Debug", &LOGGER_SHOW_DEBUG);

            if (ImGui::Button("Test Logs")) {
                spdlog::trace("test trace {}", -1);
                spdlog::debug("test debug {}", 0);
                spdlog::info("test info {}", 1);
                spdlog::warn("test warn {}", 2);
                spdlog::error("test error {}", 3);
            }

            ImGui::EndPopup();
        }

        if (ImGui::Button("Options")) {
            ImGui::OpenPopup("Options");
        }

        ImGui::Separator();

        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, 0);

        imgui_draw_logs();

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

        auto nc_val = gm->getGameVariable(GameVariable::IGNORE_DAMAGE);
        if (ImGui::Checkbox("Ignore Damage", &nc_val)) {
            gm->toggleGameVariable(GameVariable::IGNORE_DAMAGE);
        }

        auto pm_val = gm->getGameVariable(GameVariable::PRACTICE_MUSIC);
        if (ImGui::Checkbox("Practice Music", &pm_val)) {
            gm->toggleGameVariable(GameVariable::PRACTICE_MUSIC);
        }

        auto sc_val = gm->getGameVariable(GameVariable::SPEED_CONTROLS);
        if (ImGui::Checkbox("Speedhack Controls", &sc_val)) {
            gm->toggleGameVariable(GameVariable::SPEED_CONTROLS);
        }

        auto ru_val = gm->getGameVariable(GameVariable::REPLAY_CONTROLS);
        if (ImGui::Checkbox("Replay Utils", &ru_val)) {
            gm->toggleGameVariable(GameVariable::REPLAY_CONTROLS);
        }

        auto vfb_val = gm->getGameVariable(GameVariable::BYPASS_VERIFY);
        if (ImGui::Checkbox("Bypass Verify", &vfb_val)) {
            gm->toggleGameVariable(GameVariable::BYPASS_VERIFY);
        }

        auto tex_val = gm->getGameVariable(GameVariable::SHOW_TEXTURE_SETTINGS);
        if (ImGui::Checkbox("Texture Settings", &tex_val)) {
            gm->toggleGameVariable(GameVariable::SHOW_TEXTURE_SETTINGS);
        }
    }

    ImGui::End();
}

void imgui_draw_overlay_options()
{
    auto title_string = fmt::format("1.9 GDPS u{}", CMakeConfiguration::VERSION);

    ImGui::Begin(title_string.c_str(), &SHOW_DEBUG_OPTIONS, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize);

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
                SettingsPopup::create()->show();
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
    ImGui::Text("%s-%s", CMakeConfiguration::BRANCH, CMakeConfiguration::HASH);
    ImGui::Text("Development Build");

    ImGui::End();
}

void imgui_draw_log_overlay() {
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoFocusOnAppearing
            | ImGuiWindowFlags_NoNav
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoInputs
            | ImGuiWindowFlags_NoBackground;

    auto dpi = static_cast<float>(cocos2d::CCDevice::getDPI());
    auto eglview = cocos2d::CCEGLView::sharedOpenGLView();
    auto pixel_size = eglview->getFrameSize();

    auto window_pos = ImVec2(10.0f, 10.0f);
    auto window_size = ImVec2(pixel_size.width, (dpi / DPI_NORMAL_FACTOR) * 600.0f);

    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);

    if (ImGui::Begin("Log Overlay", nullptr, window_flags))
    {
        imgui_draw_logs(true);

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

    }
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

    if (SHOW_LOG_OVERLAY) {
        imgui_draw_log_overlay();
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
            ImGui::MenuItem("Logs", nullptr, &SHOW_LOG_OVERLAY);

            ImGui::EndMenuBar();
        }
    }
    ImGui::End();

    ImGui::PopStyleColor(2);
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
    spdlog::info("imgui initializing");

    if (!IS_ACTIVE) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
    }

    ImGuiIO& io = ImGui::GetIO();

    if (!IS_ACTIVE) {
        io.BackendPlatformName = "imgui_impl_cocos2dx";
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

        // official example recommends using <Load/Save>IniSettingsToMemory but i can't be bothered rn
        io.IniFilename = nullptr;
    }

    //    ImGui_ImplAndroid_Init(window);

    if (IS_ACTIVE) {
        // kill opengl3 context as window is likely invalid
        ImGui_ImplOpenGL3_Shutdown();
    }

    ImGui_ImplOpenGL3_Init("#version 300 es");

    ImGui::UpdateHoveredWindowAndCaptureFlags();

    if (IS_ACTIVE) {
        return;
    }

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
        io.Fonts->AddFontFromMemoryTTF(font_buffer, static_cast<int>(size), default_font_size);
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

            GAME_TEX = cocos2d::CCRenderTexture::create(
                static_cast<int>(pixel_size.width),
                static_cast<int>(pixel_size.height),
                cocos2d::kCCTexture2DPixelFormat_RGBA4444
            );
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

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv *env, jclass thiz, int width, int height)
{
    HookHandler::orig<&Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit>(env, thiz, width, height);

    // if issues arise we may need to add global variable
    imgui_init();
}

void CCEGLView_end(cocos2d::CCEGLView* /* self */)
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
        .add_hook("Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit", &Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit)
        .add_hook("_ZN7cocos2d10CCDirector9drawSceneEv", &CCDirector_drawScene)
        .add_hook("_ZN7cocos2d17CCTouchDispatcher7touchesEPNS_5CCSetEPNS_7CCEventEj", &CCTouchDispatcher_touches)
        .add_hook("_ZN7cocos2d9CCEGLView3endEv", &CCEGLView_end)
        .add_hook<1>("_ZN7cocos2d11CCScheduler6updateEf", &CCScheduler_update);
}
}