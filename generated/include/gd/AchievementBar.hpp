#pragma once

class TextArea;
class cocos2d;

class AchievementBar : public cocos2d::CCNodeRGBA {
public:
    virtual ~AchievementBar();
    virtual void setOpacity(unsigned char opacity);

    static AchievementBar* create(char const* title, char const* desc, char const* icon, bool quest);

    AchievementBar();
    bool init(char const* title, char const* desc, char const* icon, bool quest);
    void show();

    cocos2d::CCLayerColor* m_layerColor;
    float m_offsetUnused;
    cocos2d::CCScene* m_nextScene;
    cocos2d::extension::CCScale9Sprite* m_bg;
    cocos2d::extension::CCScale9Sprite* m_bg2;
    cocos2d::CCSprite* m_achievementGlowSprite;
    cocos2d::CCSprite* m_achievementSprite;
    cocos2d::CCLabelBMFont* m_titleLabel;
    TextArea* m_achievementDescription;

};
