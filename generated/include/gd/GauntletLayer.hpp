#pragma once

class CCMenuItemSpriteExtra;
class LoadingCircle;
class TextArea;
class cocos2d;

class GauntletLayer : public cocos2d::CCLayer, public LevelManagerDelegate {
public:
    virtual void keyBackClicked();
    virtual void loadLevelsFinished(cocos2d::CCArray* levels, char const* key, int type);
    virtual void loadLevelsFailed(char const* key, int type);

    static GauntletLayer* create(GauntletType type);
    static cocos2d::CCScene* scene(GauntletType type);

    GauntletLayer();
    ~GauntletLayer();
    bool init(GauntletType type);
    void onBack(cocos2d::CCObject* sender);
    void onLevel(cocos2d::CCObject* sender);
    void setupGauntlet(cocos2d::CCArray* levels);
    void unlockActiveItem();

    cocos2d::CCArray* m_levels;
    LoadingCircle* m_loadingCircle;
    GauntletType m_gauntletType;
    cocos2d::CCSprite* m_backgroundSprite;
    void* m_unkPtr;
    CCMenuItemSpriteExtra* m_activeItemButton;
    cocos2d::CCArray* m_activeObjects;
    TextArea* m_tryAgainText;

};
