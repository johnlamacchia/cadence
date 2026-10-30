#pragma once

class CCMenuItemSpriteExtra;
class GJGameLevel;
class WorldSelectLayer;
class cocos2d;

class GJWorldNode : public cocos2d::CCNode {
public:
    static GJWorldNode* create(int type, WorldSelectLayer* layer);

    float addDotsToLevel(int levelID, bool animate);
    cocos2d::CCPoint dotPositionForLevel(int levelID, int index);
    bool init(int type, WorldSelectLayer* layer);
    void onLevel(cocos2d::CCObject* sender);
    void playStep1();
    void playStep2();
    void playStep3();
    cocos2d::CCPoint positionForLevelButton(int levelID);
    void unlockActiveItem();

    cocos2d::CCArray* m_activeObjects;
    CCMenuItemSpriteExtra* m_activeButton;
    GJGameLevel* m_level;
    WorldSelectLayer* m_selectLayer;
    bool m_unlocked;
    cocos2d::CCPoint m_levelPosition;
    cocos2d::ccColor3B m_particleColor;
    int m_islandType;
    bool m_islandUnlocked;
    cocos2d::CCSprite* m_arrowSprite;

};
