#pragma once

class CCMenuItemSpriteExtra;
class CCMenuItemToggler;
class GJGameLevel;
class cocos2d;

class LevelCell : public TableViewCell {
public:
    virtual ~LevelCell();
    virtual bool init();
    virtual void draw();

    static LevelCell* create(float width, float height);

    LevelCell(char const* identifier, float width, float height);
    void /* TodoReturn */ inRecallMode();
    void loadCustomLevelCell();
    void loadFromLevel(GJGameLevel* level);
    void loadLocalLevelCell();
    void onClick(cocos2d::CCObject* sender);
    void onToggle(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void /* TodoReturn */ playRecallUnlock();
    void /* TodoReturn */ recallChestReady();
    void /* TodoReturn */ recallLevel();
    void /* TodoReturn */ recallSavedLevel();
    void updateBGColor(int index);
    void updateCellMode(int mode);
    void updateToggle();

    CCMenuItemSpriteExtra* m_button;
    GJGameLevel* m_level;
    bool m_cellDrawn;
    CCMenuItemToggler* m_toggler;
    cocos2d::CCPoint m_point;
    cocos2d::CCMenu* m_mainMenu;
    bool m_compactView;
    int m_cellMode;

};
