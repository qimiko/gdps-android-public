#pragma once
#ifndef _OBJECTS_GJSEARCHOBJECT_HPP
#define _OBJECTS_GJSEARCHOBJECT_HPP

#include <cocos2d.h>

class GJSearchObject : public cocos2d::CCNode {
public:
    static GJSearchObject* create(SearchType type, std::string query,
        std::string level_diff, std::string level_len,
        int page, bool star_filter,
        bool uncompleted_filter, bool featured_filter,
        int song_id_filter, bool original_filter,
        bool two_player_filter, bool custom_song_filter,
        bool enable_song_filter, bool no_star_filter);

    static GJSearchObject* createFromKey(const char*);

    static const char* getSearchKey(SearchType type, std::string query,
        std::string level_diff, std::string level_len,
        int page, bool star_filter,
        bool uncompleted_filter, bool featured_filter,
        int song_id_filter, bool original_filter,
        bool two_player_filter,
        bool custom_song_filter,
        bool enable_song_filter, bool no_star_filter);

    const char* getKey();
    const char* getNextPageKey();

    GJSearchObject* getNextPageObject();
    GJSearchObject* getPrevPageObject();

    SearchType getType() const;
    void setType(SearchType);

    std::string getString() const;
    void setString(std::string);

    std::string getDifficultyStr() const;
    void setDifficultyStr(std::string);

    std::string getLengthStr() const;
    void setLengthStr(std::string);

    int getPage() const;
    void setPage(int);

    bool getStar() const;
    void setStar(bool);

    bool getUncompleted() const;
    void setUncompleted(bool);

    bool getFeatured() const;
    void setFeatured(bool);

    int getSong() const;
    void setSong(int);

    bool getOriginal() const;
    void setOriginal(bool);

    bool getTwoP() const;
    void setTwoP(bool);

    bool getCustomSong() const;
    void setCustomSong(bool);

    bool getSongFilter() const;
    void setSongFilter(bool);

    bool getNoStar() const;
    void setNoStar(bool);
};

#endif
