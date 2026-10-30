#pragma once

class MusicChangelogEntry : public cocos2d::CCObject {
public:
    virtual ~MusicChangelogEntry();

    static MusicChangelogEntry* create(int, int, int);

    bool init(int, int, int);

};
