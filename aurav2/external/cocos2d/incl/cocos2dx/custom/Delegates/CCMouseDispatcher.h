#ifndef __CCMOUSE_DISPATCHER_H
#define __CCMOUSE_DISPATCHER_H

#include <ccMacros.h>
NS_CC_BEGIN

class CC_DLL CCMouseDispatcher {
public:
    bool dispatchScrollMSG(float, float);
};

NS_CC_END
#endif