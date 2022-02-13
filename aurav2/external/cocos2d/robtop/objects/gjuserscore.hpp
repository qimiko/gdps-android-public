#pragma once
#ifndef _OBJECTS_GJUSERSCORE_HPP
#define _OBJECTS_GJUSERSCORE_HPP

class GJUserScore : public cocos2d::CCNode {
public:
    static GJUserScore* create(cocos2d::CCDictionary*);

    CC_SYNTHESIZE(std::string, userName_, UserName);
    CC_SYNTHESIZE(std::string, userUDID_, UserUDID);
    CC_SYNTHESIZE(GJScoreType, scoreType_, ScoreType);
    CC_SYNTHESIZE(int, userID_, UserID);
    CC_SYNTHESIZE(int, accountID_, AccountID);
    CC_SYNTHESIZE(int, stars_, Stars);
    CC_SYNTHESIZE(int, demons_, Demons);
    CC_SYNTHESIZE(int, rank_, Rank);
    CC_SYNTHESIZE(int, levels_, Levels);
    CC_SYNTHESIZE(int, coins_, Coins);
    CC_SYNTHESIZE(int, icon_, Icon);
    CC_SYNTHESIZE(int, color1_, Color1);
    CC_SYNTHESIZE(int, color2_, Color2);
    CC_SYNTHESIZE(int, special_, Special);
    CC_SYNTHESIZE(int, iconType_, IconType);
};

#endif