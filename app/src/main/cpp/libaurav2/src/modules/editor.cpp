#include "modules/editor.hpp"

namespace {

    const std::string &(*string_assign)(const std::string &, const std::string &);

// persistent clipboard
    void EditorUI_destructor(EditorUI *self) {
        spdlog::info("EditorUI::~EditorUI called");

        auto string_ptr = get_from_offset<std::string>(self, 0x264);

        auto manager = GameManager::sharedState();
        bool clipboard_enabled = manager->getGameVariable(GameVariable::PERSISTENT_CLIPBOARD);
        if (clipboard_enabled) {
            manager->setEditorCopyString(string_ptr);
        }

        return HookHandler::orig<&EditorUI_destructor>(self);
    }

    bool EditorUI_init(EditorUI *self, LevelEditorLayer *editor) {
        spdlog::get("global")->info("EditorUI::init called");

        auto result = HookHandler::orig<&EditorUI_init>(self, editor);

        auto manager = GameManager::sharedState();

        auto copy_string = ptr_to_offset<std::string>(manager, 0x114);

        if (!copy_string->empty()) {
            auto string_ptr = ptr_to_offset<std::string>(self, 0x264);

            // we must go through gd if we wish to assign strings manually
            string_assign = reinterpret_cast<decltype(string_assign)>(
                    reinterpret_cast<uintptr_t>(GLOBAL_BASE) + 0x416E94);
            string_assign(*string_ptr, *copy_string);

            spdlog::trace("copy setting from `{}`", string_ptr->c_str());

            self->updateButtons();

            //		*string_ptr = copy_string;
        }
        return result;
    }

    cocos2d::CCMenu* get_menu_bar_for_index(EditorUI* self, uint32_t ind, uint32_t page)
    {
        // CCArray of EditButtonBars
        auto edit_categories = get_from_offset<cocos2d::CCArray*>(self, 0x1E0);

        auto button_bar = dynamic_cast<EditButtonBar*>(edit_categories->objectAtIndex(ind));

        // CCArray of `ButtonPage`s
        auto pages = get_from_offset<cocos2d::CCArray*>(button_bar, 0xEC);

        auto scroll_layer = reinterpret_cast<BoomScrollLayer*>(button_bar->getChildren()->objectAtIndex(0));
        auto extended_layer = reinterpret_cast<cocos2d::CCLayer*>(scroll_layer->getChildren()->objectAtIndex(0));

        auto scroll_layer_dot_node = reinterpret_cast<cocos2d::CCLayer*>(scroll_layer->getChildren()->objectAtIndex(1));
        auto scroll_layer_dot_container = get_from_offset<cocos2d::CCArray*>(scroll_layer, 0x118);

        while (pages->count() <= page) {
            // page creation stuffs
            auto last_page = dynamic_cast<ButtonPage*>(pages->lastObject());
            auto penultimate_page = dynamic_cast<ButtonPage*>(pages->objectAtIndex(pages->count() - 2));

            auto last_pos = last_page->getPosition();

            auto blank_page = ButtonPage::create(
                    cocos2d::CCArray::create(),
                    // todo, find the origin of this constant (likely changes per device?)
                    cocos2d::CCPoint(last_page->getPositionX() - penultimate_page->getPositionX(), last_pos.y)
            );

//        pages->addObject(blank_page);
            extended_layer->addChild(blank_page);

            scroll_layer->addPage(blank_page, page);

//        blank_page->setVisible(false);

            auto page_dot = cocos2d::CCSprite::create("smallDot.png");
            scroll_layer_dot_container->addObject(page_dot);
            scroll_layer_dot_node->addChild(page_dot);
        }

        auto last_page = dynamic_cast<ButtonPage*>(pages->objectAtIndex(page));

        // CCMenu of `CreateMenuItem`
        return reinterpret_cast<cocos2d::CCMenu*>(
                last_page->getChildren()->objectAtIndex(0));
    }

    void EditorUI_setupCreateMenu(EditorUI* self)
    {
        HookHandler::orig<&EditorUI_setupCreateMenu>(self);

        auto gm = GameManager::sharedState();
        auto custom_obj_enabled = gm->getGameVariable(GameVariable::SHOW_UNUSED_OBJS);
        if (!custom_obj_enabled) {
            return;
        }

        auto chain_button_menu = get_menu_bar_for_index(self, 8, 5);

        auto circle_create_btn = self->getCreateBtn(725, 4);
        auto grass_lg_create_btn = self->getCreateBtn(406, 4);
        auto grass_md_create_btn = self->getCreateBtn(407, 4);
        auto grass_sm_create_btn = self->getCreateBtn(408, 4);

        chain_button_menu->addChild(circle_create_btn);
        chain_button_menu->addChild(grass_lg_create_btn);
        chain_button_menu->addChild(grass_md_create_btn);
        chain_button_menu->addChild(grass_sm_create_btn);

        // button size is 40.0f
        // initial position is -105, -92
        auto initial_position = cocos2d::CCPoint(-105.0f, -92.0f);
        auto btn_size = 40.0f;

        circle_create_btn->setPosition(initial_position.x + (btn_size * 5),
                                       initial_position.y);
        grass_lg_create_btn->setPosition(initial_position.x,
                                         initial_position.y - btn_size);
        grass_md_create_btn->setPosition(initial_position.x + 40.0f,
                                         initial_position.y - btn_size);
        grass_sm_create_btn->setPosition(initial_position.x + (btn_size * 2),
                                         initial_position.y - btn_size);

        auto spike_button_menu = get_menu_bar_for_index(self, 4, 2);

        auto spiky_ground_create_btn = self->getCreateBtn(421, 4);
        auto spiky_ground_edge_create_btn = self->getCreateBtn(422, 4);

        spike_button_menu->addChild(spiky_ground_create_btn);
        spike_button_menu->addChild(spiky_ground_edge_create_btn);

        spiky_ground_create_btn->setPosition(initial_position.x + (btn_size * 4),
                                             initial_position.y - btn_size);
        spiky_ground_edge_create_btn->setPosition(initial_position.x + (btn_size * 5),
                                                  initial_position.y - btn_size);

        auto slope_button_menu = get_menu_bar_for_index(self, 3, 7);

        auto short_slope_btn = self->getCreateBtn(371, 4);
        auto long_slope_btn = self->getCreateBtn(372, 4);
        auto short_slope_corner_btn = self->getCreateBtn(373, 4);
        auto long_slope_corner_btn = self->getCreateBtn(374, 4);

        slope_button_menu->addChild(short_slope_btn);
        slope_button_menu->addChild(long_slope_btn);
        slope_button_menu->addChild(short_slope_corner_btn);
        slope_button_menu->addChild(long_slope_corner_btn);

        short_slope_btn->setPosition(initial_position.x + (btn_size * 0),
                                     initial_position.y);
        long_slope_btn->setPosition(initial_position.x + (btn_size * 1),
                                    initial_position.y);
        short_slope_corner_btn->setPosition(initial_position.x + (btn_size * 2),
                                            initial_position.y);
        long_slope_corner_btn->setPosition(initial_position.x + (btn_size * 3),
                                           initial_position.y);

        auto ground_menu_first = get_menu_bar_for_index(self, 7, 1);

        auto spiky_deco_btn = self->getCreateBtn(419, 4);
        ground_menu_first->addChild(spiky_deco_btn);

        spiky_deco_btn->setPosition(initial_position.x + (btn_size * 5),
                                    initial_position.y - btn_size);

        auto ground_menu_second = get_menu_bar_for_index(self, 7, 2);

        auto spiky_deco_edge_btn = self->getCreateBtn(420, 4);
        ground_menu_second->addChild(spiky_deco_edge_btn);

        spiky_deco_edge_btn->setPosition(initial_position.x + (btn_size * 0),
                                         initial_position.y);

        return;
    }

    bool ColorSelectPopup_init(ColorSelectPopup* self, GameObject* target, int color_id, int player_id, int blending) {
        auto use_advanced_select = GameManager::sharedState()->getGameVariable(GameVariable::ADVANCED_COLOR_SELECT);
        if (use_advanced_select) {
            HookHandler::get_handler().install_patch(0x48BFB9, { '2' }); // set fadetime label precision
        } else {
            HookHandler::get_handler().install_patch(0x48BFB9, { '1' });
        }

        if (HookHandler::orig<&ColorSelectPopup_init>(self, target, color_id, player_id, blending)) {
            auto ccd = cocos2d::CCDirector::sharedDirector();

            if (use_advanced_select) {
                auto adv_widget = AdvancedColorSelectWidget::create(self);
                self->getInternalLayer()->addChild(adv_widget);
                adv_widget->setPosition(ccd->getScreenLeft() + 40.0f, ccd->getScreenBottom() + 150.0f);

                self->setUserObject(adv_widget);
            }

            return true;
        }

        return false;
    }

    void ColorSelectPopup_colorValueChanged(ColorSelectPopup* self, cocos2d::ccColor3B col) {
        HookHandler::orig<&ColorSelectPopup_colorValueChanged>(self, col);

        if (self->getUserObject() != nullptr) {
            auto color_select = static_cast<AdvancedColorSelectWidget*>(self->getUserObject());

            if (!color_select->getIsInUpdate()) {
                color_select->updateColorInputs(true);
            }
        }
    }
}

namespace Editor {
    void Module::on_initialize()
    {
        HookHandler::get_handler()
                .add_hook(&EditorUI::setupCreateMenu, EditorUI_setupCreateMenu)
                .add_hook(&ColorSelectPopup::init, &ColorSelectPopup_init)
                .add_hook("_ZN16ColorSelectPopup17colorValueChangedEN7cocos2d10_ccColor3BE", &ColorSelectPopup_colorValueChanged)
                .add_hook("_ZN8EditorUID0Ev", EditorUI_destructor)
                .add_hook("_ZN8EditorUI4initEP16LevelEditorLayer", EditorUI_init);
    }
} // namespace Editor
