#ifndef __CCKEYBOARD_DISPATCHER_H
#define __CCKEYBOARD_DISPATCHER_H

#include <ccMacros.h>
NS_CC_BEGIN

enum enumKeyCodes;
class CC_DLL CCKeyboardDispatcher {
public:
	bool dispatchKeyboardMSG(enumKeyCodes, bool);
  void updateModifierKeys(bool shiftKeyPressed, bool controlKeyPressed, bool altKeyPressed, bool commandKeyPressed);

  bool getShiftKeyPressed() const;
	bool getAltKeyPressed() const;
	bool getControlKeyPressed() const;
	bool getCommandKeyPressed() const;
};

NS_CC_END
#endif