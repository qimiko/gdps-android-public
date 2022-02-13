#pragma once
#ifndef _MANAGERS_GJACCOUNTMANAGER_HPP
#define _MANAGERS_GJACCOUNTMANAGER_HPP

#include <cocos2d.h>
#include <string>

class GJAccountManager : public cocos2d::CCNode {
public:
    static GJAccountManager* sharedState();
    void dataLoaded(DS_Dictionary*);
    void encodeDataTo(DS_Dictionary*);

    int getAccountID() const;
    std::string getAccountPassword() const;
    std::string getAccountUserName() const;

    void unlinkFromAccount();
    void onLoginAccountCompleted(std::string, std::string);
};

#endif
