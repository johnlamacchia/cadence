#pragma once

class cocos2d;

class SFXChangelogPopup : public ScrollViewPopup {
public:
    virtual ~SFXChangelogPopup();
    virtual float buildContent(cocos2d::CCNode*, float, float);

    static SFXChangelogPopup* create(cocos2d::CCArray*);

    bool init(cocos2d::CCArray*);
    void onCredits(cocos2d::CCObject* sender);

};
