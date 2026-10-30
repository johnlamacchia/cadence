#pragma once

class cocos2d;

class MusicChangelogPopup : public ScrollViewPopup {
public:
    virtual ~MusicChangelogPopup();
    virtual float buildContent(cocos2d::CCNode*, float, float);

    static MusicChangelogPopup* create(cocos2d::CCArray*);

    bool init(cocos2d::CCArray*);

};
