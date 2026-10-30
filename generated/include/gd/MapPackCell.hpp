#pragma once

class CCMenuItemSpriteExtra;
class GJMapPack;
class cocos2d;

class MapPackCell : public TableViewCell {
public:
    virtual bool init();
    virtual void draw();

    MapPackCell(char const* identifier, float width, float height);
    ~MapPackCell();
    void loadFromMapPack(GJMapPack* pack);
    void onClaimReward(cocos2d::CCObject* sender);
    void onClick(cocos2d::CCObject* sender);
    void playCompleteEffect();
    void reloadCell();
    void updateBGColor(int index);

    GJMapPack* m_mapPack;
    CCMenuItemSpriteExtra* m_viewButton;
    CCMenuItemSpriteExtra* m_rewardButton;
    cocos2d::CCArray* m_rewardLabels;
    cocos2d::CCArray* m_rewardSprites;

};
