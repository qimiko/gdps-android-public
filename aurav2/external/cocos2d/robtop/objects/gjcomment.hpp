#pragma once
#ifndef _OBJECTS_GJCOMMENT_HPP
#define _OBJECTS_GJCOMMENT_HPP

class GJComment : public cocos2d::CCNode {
public:
    static GJComment* create(cocos2d::CCDictionary* dict);

    CC_SYNTHESIZE(std::string, comment_, Comment);
    CC_SYNTHESIZE(std::string, userName_, UserName);
    CC_SYNTHESIZE(long, commentID_, CommentID);
    CC_SYNTHESIZE(int, userID_, UserID);
    CC_SYNTHESIZE(int, likes_, Likes);
    CC_SYNTHESIZE(int, dislikes_, Dislikes);
    CC_SYNTHESIZE(int, levelID_, LevelID);
    CC_SYNTHESIZE(bool, isHidden_, IsHidden);
    CC_SYNTHESIZE(int, accountID_, AccountID);
};

#endif
