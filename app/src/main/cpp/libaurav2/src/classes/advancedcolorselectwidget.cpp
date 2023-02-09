#include "classes/advancedcolorselectwidget.hpp"

AdvancedColorSelectWidget* AdvancedColorSelectWidget::create(ColorSelectPopup* target) {
    auto p_ret = new (std::nothrow) AdvancedColorSelectWidget();
    if (p_ret && p_ret->init(target)) {
        p_ret->autorelease();
        return p_ret;
    }
    else {
        delete p_ret;
        p_ret = nullptr;
        return nullptr;
    }
}

bool AdvancedColorSelectWidget::init(ColorSelectPopup* target) {
    this->target_ = target;
    auto color = target_->getColorValue();

    // this should be useless as the inputs will be null
    is_in_update_ = true;

    {
        auto color_input = CCTextInputNode::create(100.0f, 50.0f, "R", "Thonburi", 24, "bigFont.fnt");
        this->addChild(color_input, 4);
        color_input->setDelegate(this);
        color_input->setCharLimit(3);
        color_input->setAllowedChars("0123456789");
        color_input->setMaxLabelWidth(50.0f);
        color_input->setMaxLabelScale(0.6f);
        color_input->setLabelPlaceholderScale(0.6f);
        color_input->setLabelPlaceholderColor({120, 170, 240 });
        color_input->setPosition(0.0f, 70.0f);
        this->color_red_ = color_input;

        auto color_label = cocos2d::CCLabelBMFont::create("R", "bigFont.fnt");
        this->addChild(color_label);
        color_label->setPosition(40.0f, 70.0f);
        color_label->setScale(0.50f);

        color_input->setString(fmt::format("{}", color.r));

        auto label_bg = cocos2d::extension::CCScale9Sprite::create(
                "square02_small.png", cocos2d::CCRect(0.0f, 0.0f, 40.0f, 40.0f));
        label_bg->setContentSize({ 50.0f, 30.0f });
        label_bg->setOpacity(100);
        label_bg->setPosition(0.0f, 70.0f);
        this->addChild(label_bg);
    }

    {
        auto color_input = CCTextInputNode::create(100.0f, 50.0f, "G", "Thonburi", 24, "bigFont.fnt");
        this->addChild(color_input, 4);
        color_input->setDelegate(this);
        color_input->setCharLimit(3);
        color_input->setAllowedChars("0123456789");
        color_input->setMaxLabelWidth(50.0f);
        color_input->setMaxLabelScale(0.6f);
        color_input->setLabelPlaceholderScale(0.6f);
        color_input->setLabelPlaceholderColor({120, 170, 240 });
        color_input->setPosition(0.0f, 35.0f);
        this->color_green_ = color_input;

        auto color_label = cocos2d::CCLabelBMFont::create("G", "bigFont.fnt");
        this->addChild(color_label);
        color_label->setPosition(40.0f, 35.0f);
        color_label->setScale(0.50f);

        color_input->setString(fmt::format("{}", color.g));

        auto label_bg = cocos2d::extension::CCScale9Sprite::create(
                "square02_small.png", cocos2d::CCRect(0.0f, 0.0f, 40.0f, 40.0f));
        label_bg->setContentSize({ 50.0f, 30.0f });
        label_bg->setOpacity(100);
        label_bg->setPosition(0.0f, 35.0f);
        this->addChild(label_bg);
    }

    {
        auto color_input = CCTextInputNode::create(100.0f, 50.0f, "B", "Thonburi", 24, "bigFont.fnt");
        this->addChild(color_input, 4);
        color_input->setDelegate(this);
        color_input->setCharLimit(3);
        color_input->setAllowedChars("0123456789");
        color_input->setMaxLabelWidth(50.0f);
        color_input->setMaxLabelScale(0.6f);
        color_input->setLabelPlaceholderScale(0.6f);
        color_input->setLabelPlaceholderColor({120, 170, 240 });
        color_input->setPosition(0.0f, 0.0f);
        this->color_blue_ = color_input;

        auto color_label = cocos2d::CCLabelBMFont::create("B", "bigFont.fnt");
        this->addChild(color_label);
        color_label->setPosition(40.0f, 0.0f);
        color_label->setScale(0.50f);

        color_input->setString(fmt::format("{}", color.b));

        auto label_bg = cocos2d::extension::CCScale9Sprite::create(
                "square02_small.png", cocos2d::CCRect(0.0f, 0.0f, 40.0f, 40.0f));
        label_bg->setContentSize({ 50.0f, 30.0f });
        label_bg->setOpacity(100);
        label_bg->setPosition(0.0f, 0.0f);
        this->addChild(label_bg);
    }

    is_in_update_ = false;

    updateColorInputs();

    return true;
}

void AdvancedColorSelectWidget::updateColors() {
    auto red_string = color_red_->getTextField()->getString();
    auto red = std::atoi(red_string);

    auto green_string = color_green_->getTextField()->getString();
    auto green = std::atoi(green_string);

    auto blue_string = color_blue_->getTextField()->getString();
    auto blue = std::atoi(blue_string);

    // clamp values
    red = std::max(std::min(red, 255), 0);
    green = std::max(std::min(green, 255), 0);
    blue = std::max(std::min(blue, 255), 0);

    target_->selectColor({
                                 static_cast<uint8_t>(red),
                                 static_cast<uint8_t>(green),
                                 static_cast<uint8_t>(blue)
                         });

    this->updateColorInputs();
}

void AdvancedColorSelectWidget::updateColorInputs(bool ignore_changes) {
    auto color = target_->getColorValue();

    if (ignore_changes) {
        is_in_update_ = true;
    }

    color_red_->setString(fmt::format("{}", color.r));
    color_green_->setString(fmt::format("{}", color.g));
    color_blue_->setString(fmt::format("{}", color.b));

    if (ignore_changes) {
        is_in_update_ = false;
    }
}

void AdvancedColorSelectWidget::textChanged(CCTextInputNode*) {
    if (!color_red_ || !color_blue_ || !color_green_) {
        return;
    }

    if (!is_in_update_) {
        is_in_update_ = true;
        updateColors();
        is_in_update_ = false;
    }
}

bool AdvancedColorSelectWidget::getIsInUpdate() const {
    return is_in_update_;
}
