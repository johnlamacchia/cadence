#pragma once

class cocos2d;

class GJBigSprite : public cocos2d::CCNode {
public:
    virtual bool init();

    static GJBigSprite* create();

    void finishedLoadingSpriteAsync(cocos2d::CCObject* obj);
    void loadSpriteAsync(std::string filename, int index);
    void unloadAll();
    void unloadSprite(std::string filename, int index);
    void updateSpriteVisibility();

    std::vector<bool> m_spritesLoaded;
    cocos2d::CCArray* m_spriteNodes;
    float m_scaleX;
    float m_scaleY;
    bool m_loadSprites;
    bool m_unloadingAll;

};
