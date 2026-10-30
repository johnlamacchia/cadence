#pragma once

class FontObject;
class cocos2d;

class BitmapFontCache : public cocos2d::CCObject {
public:
    static void purgeSharedFontCache();
    static BitmapFontCache* sharedFontCache();

    BitmapFontCache();
    ~BitmapFontCache();
    FontObject* fontWithConfigFile(char const* font, float scale);
    bool init();

    cocos2d::CCDictionary* m_cache;

};
