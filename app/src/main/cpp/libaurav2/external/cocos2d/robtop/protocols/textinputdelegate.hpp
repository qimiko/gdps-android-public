#pragma once

#ifndef _PROTOCOLS_TEXTINPUTDELEGATE_HPP
#define _PROTOCOLS_TEXTINPUTDELEGATE_HPP

class CCTextInputNode;

class TextInputDelegate {
    virtual void textChanged(CCTextInputNode*);
    virtual void textInputOpened(CCTextInputNode*);
    virtual void textInputClosed(CCTextInputNode*);
    virtual void textInputShouldOffset(CCTextInputNode*, float);
    virtual void textInputReturn(CCTextInputNode*);
    virtual bool allowTextInput(CCTextInputNode*);
};

#endif //_PROTOCOLS_TEXTINPUTDELEGATE_HPP
