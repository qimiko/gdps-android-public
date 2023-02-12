#include "modules/levels.hpp"

namespace {
std::string GetAudioTitle(int audio_id)
{
    spdlog::trace("running getaudiotitle for id {}", audio_id);

    switch (audio_id) {
    case 18:
        return "Thumper";
    case 19:
        return "Clutterfunk 2";
    case 20:
        return "Aura";
    case 21:
        return "Jack Russel";
    default:
        return HookHandler::orig<&GetAudioTitle>(audio_id);
    }
}

int LevelTools_artistForAudio(int audio_id)
{
    spdlog::trace("running artistforaudio for id {}", audio_id);

    /* 0 - DJVI
     * 1 - Waterflame
     * 2 - OcularNebula
     * 3 - ForeverBound
     * 5 - DJ-Nate
     * 6 - Creo
     * 7 - Bossfight
     * */

    if (audio_id == -1) {
        return 2;
    }

    constexpr std::array<int, 23> audioArtists = {
            3, 0, 4, 0, 0, 0, 1, 1, 0, 0, 1, 5, 1, 5, 5, 1, 1, 5, 1, 1, 6, 7
    };

    if (audio_id > audioArtists.size()) {
        return 0;
    }

    return audioArtists.at(audio_id);
}

std::string LevelTools_urlForAudio(int audio_id)
{
    spdlog::trace("running urlforaudio for id {}", audio_id);

    switch (audio_id) {
    case 18:
        return "https://www.youtube.com/watch?v=Rge_r3Eri0Q";
    case 19:
        return "https://www.youtube.com/watch?v=YFrimAIrU8I";
    case 20:
        return "https://www.youtube.com/watch?v=LXTrXaYMgYs";
    case 21:
        return "https://www.youtube.com/watch?v=PteZ2d3UeTo";
    default:
        return HookHandler::orig<&LevelTools_urlForAudio>(audio_id);
    }
}

std::string LevelTools_nameForArtist(int artist_id)
{
    spdlog::trace("running nameforartist for id {}", artist_id);

    switch (artist_id) {
    case 6:
        return "Creo";
    case 7:
        return "Bossfight";
    default:
        return HookHandler::orig<&LevelTools_nameForArtist>(artist_id);
    }
}

std::string LevelTools_ngURLForArtist(int artist_id)
{
    spdlog::trace("running ngurlforartist for id {}", artist_id);

    switch (artist_id) {
    case 6:
        return "https://creomusic.newgrounds.com";
    case 7:
        return "https://bossfightofficial.newgrounds.com";
    default:
        return HookHandler::orig<&LevelTools_ngURLForArtist>(artist_id);
    }
}

std::string LevelTools_ytURLForArtist(int artist_id)
{
    spdlog::trace("running ngurlforartist for id {}", artist_id);

    switch (artist_id) {
    case 6:
        return "http://www.youtube.com/creomusic";
    case 7:
        return "https://www.youtube.com/c/Bossfightmusic";
    default:
        return HookHandler::orig<&LevelTools_ytURLForArtist>(artist_id);
    }
}

std::string GetAudioFileName(int audio_id)
{
    spdlog::trace("running getaudiofilename for id {}", audio_id);

    switch (audio_id) {
    case 18:
        return "Thumper.mp3";
    case 19:
        return "Clutterfunk2.mp3";
    case 20:
        return "Aura.mp3";
    case 21:
        return "JackRussel.mp3";
    default:
        return HookHandler::orig<&GetAudioFileName>(audio_id);
    }
}

GJGameLevel* GetLevel(int level_id, bool p1)
{
    spdlog::trace("running getlevel for level id {}, p1 is {} (lvl {})",
            level_id, p1,
            LevelTools::getAudioTitle(level_id - 1).c_str());

    switch (level_id) {
    case 19: { // getlevel ids are +1 the song id (for some odd reason)
        // thumper (which thinks that it's stereo madness)
        GJGameLevel* level = GJGameLevel::create();
        int song_id = level_id - 1;

        level->setLevelID(level_id);

        level->setLevelName(LevelTools::getAudioTitle(song_id));
        level->setAudioTrack(song_id);

        level->setStars(10);
        level->setDifficulty(4);

        // we'll use this if we're making our own gamelevel from scratch
        level->setLevelType(GJLevelType::Local);
        level->setCoins(3);

        LocalLevelManager* llm = LocalLevelManager::sharedState();
        std::string level_string = llm->getMainLevelString(level_id);

        level->setLevelString(level_string);
        return level;
    }
    case 20: {
        // clutterfunk 2
        GJGameLevel* level = GJGameLevel::create();
        int song_id = level_id - 1;

        level->setLevelID(level_id);

        level->setDemon(true);

        level->setLevelName(LevelTools::getAudioTitle(song_id));
        level->setAudioTrack(song_id);

        level->setStars(16);
        level->setDifficulty(6);

        level->setLevelType(GJLevelType::Local);
        level->setCoins(3);

        LocalLevelManager* llm = LocalLevelManager::sharedState();
        std::string level_string = llm->getMainLevelString(level_id);

        level->setLevelString(level_string);
        return level;
    }
    case 21: {
        // aura
        GJGameLevel* level = GJGameLevel::create();
        int song_id = level_id - 1;

        level->setLevelID(level_id);

        level->setLevelName(LevelTools::getAudioTitle(song_id));
        level->setAudioTrack(song_id);

        level->setStars(11);
        level->setDifficulty(5);

        level->setLevelType(GJLevelType::Local);
        level->setCoins(3);

        LocalLevelManager* llm = LocalLevelManager::sharedState();
        std::string level_string = llm->getMainLevelString(level_id);

        level->setLevelString(level_string);
        return level;
    }
    case 22: {
        // jack russel
        GJGameLevel* level = GJGameLevel::create();
        int song_id = level_id - 1;

        level->setLevelID(level_id);

        level->setLevelName(LevelTools::getAudioTitle(song_id));
        level->setAudioTrack(song_id);

        level->setStars(12);
        level->setDifficulty(5);

        level->setLevelType(GJLevelType::Local);
        level->setCoins(3);

        LocalLevelManager* llm = LocalLevelManager::sharedState();
        std::string level_string = llm->getMainLevelString(level_id);

        level->setLevelString(level_string);
        return level;
    }
    default:
        return HookHandler::orig<&GetLevel>(level_id, p1);
    }
}

void MenuLayer_onPlay(MenuLayer* self, cocos2d::CCObject* target)
{
    HookHandler::orig<&MenuLayer_onPlay>(self, target);

    auto glm = GameLevelManager::sharedState();

    spdlog::trace("running level coins migration (oops)");
    for (auto i = 19u; i <= 22; ++i) {
        // so i accidentally introduced a bug
        // this bug breaks save data, oops
        // so here i fix the data
        auto c_ml = glm->getMainLevel(i, false);
        c_ml->setLevelType(GJLevelType::Local);
    }
}

// stole this from mat btw (hi mat)
void LevelInfoLayer_onClone(LevelInfoLayer* self, cocos2d::CCObject* target) {
    // this value gets set after onclone is run
    auto run_guard = get_from_offset<bool>(self, 0x134);
    HookHandler::orig<&LevelInfoLayer_onClone>(self, target);

    if (!self->shouldDownloadLevel() && !run_guard) {
        auto level = get_from_offset<GJGameLevel*>(self, 0x13C);
        auto new_level = static_cast<GJGameLevel*>(
                LocalLevelManager::sharedState()->getLocalLevels()->objectAtIndex(0));
        new_level->setSongID(level->getSongID());
    }
}

void EditLevelLayer_onClone(EditLevelLayer* self) {
    auto run_guard = get_from_offset<bool>(self, 0x130);
    HookHandler::orig<&EditLevelLayer_onClone>(self);

    if (!run_guard) {
        auto level = get_from_offset<GJGameLevel*>(self, 0x124);
        auto new_level = static_cast<GJGameLevel*>(
                LocalLevelManager::sharedState()->getLocalLevels()->objectAtIndex(0));
        new_level->setSongID(level->getSongID());
    }
}


} // namespace

namespace Levels {
void Module::on_initialize()
{
    constexpr uint8_t max_lvls = 22;
    constexpr uint8_t max_songs = 22;

    constexpr uint8_t max_lvl_p1 = max_lvls + 1;
    constexpr uint8_t max_song_m1 = max_songs - 1;

    HookHandler::get_handler()
        .add_hook(&LevelTools::getAudioTitle, GetAudioTitle)
        .add_hook(&LevelTools::getAudioFileName, GetAudioFileName)
        .add_hook(&LevelTools::getLevel, GetLevel)
        .add_hook(&LevelTools::artistForAudio, LevelTools_artistForAudio)
        .add_hook(&LevelTools::nameForArtist, LevelTools_nameForArtist)
        .add_hook(&LevelTools::ngURLForArtist, LevelTools_ngURLForArtist)
        .add_hook(&LevelTools::ytURLForArtist, LevelTools_ytURLForArtist)
        .add_hook(&LevelTools::urlForAudio, LevelTools_urlForAudio)
        .add_hook(&MenuLayer::onPlay, MenuLayer_onPlay)
        .add_hook(&LevelInfoLayer::onClone, LevelInfoLayer_onClone)
        .add_hook(&EditLevelLayer::onClone, EditLevelLayer_onClone)
        .install_patch(0x20D09C, { max_lvl_p1 }) // cmp i, 0x13
        .install_patch(0x1F216C, { max_songs }) // cmp audio_id, 0x12
        .install_patch(0x1F2108, { max_songs }) // cmp audio_id, 0x12
        .install_patch(0x1F210E, { max_songs }) // mov.ge song_id, 0x12
        .install_patch(0x218B34, { max_songs }) // cmp tmp, 0x12
        .install_patch(0x1DE2F0, { max_song_m1 }) // cmp audio_id, 0x11
        .install_patch(0x1DE274, { max_song_m1 }) // cmp audio_id, 0x11
        .install_patch(0x1DE27A, { max_song_m1 }); // cmp audio_id, 0x11
}
} // namespace Levels