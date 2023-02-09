#pragma once

#ifndef CLASSES_ADVANCEDCOLORSELECTWIDGET_HPP
#define CLASSES_ADVANCEDCOLORSELECTWIDGET_HPP

#include <cocos2dcpp.h>
#include <fmt/format.h>

class AdvancedColorSelectWidget : public cocos2d::CCLayer, TextInputDelegate {
public:
    static AdvancedColorSelectWidget* create(ColorSelectPopup* target);

private:
    ColorSelectPopup* target_{nullptr};
    CCTextInputNode* color_red_{nullptr};
    CCTextInputNode* color_green_{nullptr};
    CCTextInputNode* color_blue_{nullptr};

    bool is_in_update_{false};

    void updateColors();

public:
    bool getIsInUpdate() const;

    void updateColorInputs(bool ignore_changes = false);

    void textChanged(CCTextInputNode*) override;

    bool init(ColorSelectPopup* target);
};

#endif //CLASSES_ADVANCEDCOLORSELECTWIDGET_HPP
