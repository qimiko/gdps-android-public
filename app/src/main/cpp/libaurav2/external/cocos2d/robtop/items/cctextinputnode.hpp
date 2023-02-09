#pragma once
#ifndef _ITEMS_CCTEXTINPUTNODE_HPP
#define _ITEMS_CCTEXTINPUTNODE_HPP

#include <cocos2d.h>

#include <robtop/protocols/textinputdelegate.hpp>

class CCTextInputNode :
        public cocos2d::CCLayer,public cocos2d::CCIMEDelegate, public cocos2d::CCTextFieldDelegate {
public:
    static CCTextInputNode* create(float width, float height, const char* label,
                                   const char* labelFont, int, const char* fieldFont);

    void setDelegate(TextInputDelegate*);
    TextInputDelegate* getDelegate() const;

    void setCharLimit(int);

    void setMaxLabelScale(float);
    void setMaxLabelWidth(float);
    void setAllowedChars(std::string);
    void setLabelPlaceholderColor(cocos2d::ccColor3B);
    void setLabelPlaceholderScale(float);

    void setString(std::string);
    std::string getString();

    cocos2d::CCTextFieldTTF* getTextField() const;
};

#endif //_ITEMS_CCTEXTINPUTNODE_HPP
