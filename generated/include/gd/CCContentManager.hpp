#pragma once

class cocos2d;

class CCContentManager : public cocos2d::CCObject {
public:
    static CCContentManager* sharedManager();

    cocos2d::CCDictionary* addDict(char const*, bool);
    cocos2d::CCDictionary* addDictDS(char const*);
    void clearCache();
    bool init();

};
