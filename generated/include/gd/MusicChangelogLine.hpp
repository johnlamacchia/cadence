#pragma once

class MusicChangelogLine : public cocos2d::CCObject {
public:
    virtual ~MusicChangelogLine();

    static MusicChangelogLine* create(int, std::string, int);

    bool init(int, std::string, int);

};
