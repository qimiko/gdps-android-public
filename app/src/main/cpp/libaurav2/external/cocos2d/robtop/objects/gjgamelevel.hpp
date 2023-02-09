#pragma once
#ifndef _OBJECTS_GJGAMELEVEL_HPP
#define _OBJECTS_GJGAMELEVEL_HPP

#include <cocos2d.h>

class GJGameLevel : public cocos2d::CCNode {
public:
    static GJGameLevel* create();

    CC_SYNTHESIZE_RETAIN(cocos2d::CCDictionary*, lastBuildSave_, LastBuildSave);
    CC_SYNTHESIZE(int, levelID_, LevelID);
    CC_SYNTHESIZE(std::string, levelName_, LevelName);
    CC_SYNTHESIZE(std::string, levelDesc_, LevelDesc);
    CC_SYNTHESIZE(std::string, levelString_, LevelString);
    CC_SYNTHESIZE(std::string, userName_, UserName);
    CC_SYNTHESIZE(std::string, recordString_, RecordString);
    CC_SYNTHESIZE(std::string, uploadDate_, UploadDate);
    CC_SYNTHESIZE(std::string, updateDate_, UpdateDate);
    CC_SYNTHESIZE(int, userID_, UserID);
    CC_SYNTHESIZE(int, difficulty_, Difficulty);
    CC_SYNTHESIZE(int, audioTrack_, AudioTrack);
    CC_SYNTHESIZE(int, songID_, SongID);
    CC_SYNTHESIZE(int, levelRev_, LevelRev);
    CC_SYNTHESIZE(int, objectCount_, ObjectCount);
    CC_SYNTHESIZE(int, order_, Order);
    CC_SYNTHESIZE(int, ratings_, Ratings);
    CC_SYNTHESIZE(int, ratingsSum_, RatingsSum);
    CC_SYNTHESIZE(int, downloads_, Downloads);
    CC_SYNTHESIZE(int, completes_, Completes);
    CC_SYNTHESIZE(bool, isEditable_, IsEditable);
    CC_SYNTHESIZE(bool, isVerified_, IsVerified);
    CC_SYNTHESIZE(bool, isUploaded_, IsUploaded);
    CC_SYNTHESIZE(bool, hasBeenModified_, HasBeenModified);
    CC_SYNTHESIZE(int, levelVersion_, LevelVersion);
    CC_SYNTHESIZE(int, gameVersion_, GameVersion);
    CC_SYNTHESIZE(int, attempts_, Attempts);
    CC_SYNTHESIZE(int, jumps_, Jumps);
    CC_SYNTHESIZE(int, normalPercent_, NormalPercent);
    CC_SYNTHESIZE(int, practicePercent_, PracticePercent);
    CC_SYNTHESIZE(int, likes_, Likes);
    CC_SYNTHESIZE(int, dislikes_, Dislikes);
    CC_SYNTHESIZE(int, levelLength_, LevelLength);
    CC_SYNTHESIZE(int, featured_, Featured);
    CC_SYNTHESIZE(bool, demon_, Demon);
    CC_SYNTHESIZE(int, stars_, Stars);
    CC_SYNTHESIZE(bool, autoLevel_, AutoLevel);
    CC_SYNTHESIZE(int, coins_, Coins);
    CC_SYNTHESIZE(int, password_, Password);
    CC_SYNTHESIZE(int, originalLevel_, OriginalLevel);
    CC_SYNTHESIZE(bool, twoPlayerMode_, TwoPlayerMode);
    CC_SYNTHESIZE(int, failedPasswordAttempts_, FailedPasswordAttempts);
    CC_SYNTHESIZE(bool, showedSongWarning_, ShowedSongWarning);
    CC_SYNTHESIZE(int, starRatings_, StarRatings);
    CC_SYNTHESIZE(int, starRatingsSum_, StarRatingsSum);
    CC_SYNTHESIZE(int, maxStarRatings_, MaxStarRatings);
    CC_SYNTHESIZE(int, minStarRatings_, MinStarRatings);
    CC_SYNTHESIZE(int, demonVotes_, DemonVotes);
    CC_SYNTHESIZE(int, rateStars_, RateStars);
    CC_SYNTHESIZE(bool, rateFeature_, RateFeature);
    CC_SYNTHESIZE(std::string, rateUser_, RateUser);
    CC_SYNTHESIZE(bool, dontSave_, DontSave);
    CC_SYNTHESIZE(bool, isHidden_, IsHidden);
    CC_SYNTHESIZE(int, requiredCoins_, RequiredCoins);
    CC_SYNTHESIZE(bool, isUnlocked_, IsUnlocked);
    CC_SYNTHESIZE(cocos2d::CCPoint, lastCameraPos_, LastCameraPos);
    CC_SYNTHESIZE(float, lastEditorZoom_, LastEditorZoom);
    CC_SYNTHESIZE(int, lastBuildTab_, LastBuildTab);
    CC_SYNTHESIZE(int, lastBuildPage_, LastBuildPage);
    CC_SYNTHESIZE(int, lastBuildGroupID_, LastBuildGroupID);
    CC_SYNTHESIZE(GJLevelType, levelType_, LevelType);
    CC_SYNTHESIZE_READONLY(int, m_ID_, M_ID);
    CC_SYNTHESIZE(std::string, tempName_, TempName);
    CC_SYNTHESIZE(int, capacity001_, Capacity001);
    CC_SYNTHESIZE(int, capacity002_, Capacity002);
    CC_SYNTHESIZE(int, capacity003_, Capacity003);
    CC_SYNTHESIZE(int, capacity004_, Capacity004);
};

#endif
