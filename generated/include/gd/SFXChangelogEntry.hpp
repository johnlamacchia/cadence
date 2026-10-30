#pragma once

class SFXChangelogEntry : public cocos2d::CCObject {
public:
    virtual ~SFXChangelogEntry();

    static SFXChangelogEntry* create(int, int, int);

    bool init(int, int, int);

};
