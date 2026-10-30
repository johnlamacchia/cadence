#pragma once

class cocos2d;

class GJItemIcon : public cocos2d::CCSprite {
public:
    virtual void setOpacity(unsigned char opacity);

    static GJItemIcon* create(UnlockType type, int id, cocos2d::ccColor3B color1, cocos2d::ccColor3B color2, bool dark, bool unused, bool noLabel, cocos2d::ccColor3B unlockColor);
    static GJItemIcon* createBrowserItem(UnlockType unlockType, int itemID);
    static GJItemIcon* createStoreItem(UnlockType type, int id, bool dark, cocos2d::ccColor3B unlockColor);
    static float scaleForType(UnlockType type);
    static cocos2d::ccColor3B unlockedColorForType(int type);

    GJItemIcon();
    ~GJItemIcon();
    void changeToLockedState(float scale);
    void darkenStoreItem(ShopType type);
    void darkenStoreItem(cocos2d::ccColor3B color);
    bool init(UnlockType type, int id, cocos2d::ccColor3B color1, cocos2d::ccColor3B color2, bool dark, bool unused, bool noLabel, cocos2d::ccColor3B unlockColor);
    void toggleEnabledState(bool enabled);

    cocos2d::CCSprite* m_player;
    int m_iconRequestID;
    cocos2d::CCSize m_playerSize;
    bool m_isIcon;
    int m_unlockID;
    UnlockType m_unlockType;

};
