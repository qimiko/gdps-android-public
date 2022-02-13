#pragma once
#ifndef _MANAGERS_MUSICDOWNLOADMANAGER_HPP
#define _MANAGERS_MUSICDOWNLOADMANAGER_HPP

class MusicDownloadManager {
public:
    static MusicDownloadManager* sharedState();
    bool isSongDownloaded(int);
    void deleteSong(int);

    void songStateChanged();
};

#endif
