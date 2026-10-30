#pragma once

class DS_Dictionary;
class cocos2d;

class CCAnimateFrameCache : public cocos2d::CCObject {
public:
    virtual ~CCAnimateFrameCache();

    static CCAnimateFrameCache* get();
    static CCAnimateFrameCache* sharedSpriteFrameCache();

    void addCustomSpriteFramesWithFile(char const* filename);
    cocos2d::CCDictionary* addDict(DS_Dictionary* dict, char const* filename);
    cocos2d::CCDictionary* addDict(cocos2d::CCDictionary* dict, char const* filename);
    cocos2d::CCDictionary* addSpriteFramesWithFile(char const* filename);
    bool init();
    void purgeSharedSpriteFrameCache();
    void removeSpriteFrames();
    cocos2d::CCArray* spriteFrameByName(char const* name);

    cocos2d::CCDictionary* m_animateFrames;
    cocos2d::CCDictionary* m_unk040;
    cocos2d::CCDictionary* m_animateDescriptions;

};
