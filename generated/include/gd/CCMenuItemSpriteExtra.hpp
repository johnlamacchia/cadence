#pragma once

class cocos2d;

class CCMenuItemSpriteExtra : public cocos2d::CCMenuItemSprite {
public:
    virtual void activate();
    virtual void selected();
    virtual void unselected();

    static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
    static CCMenuItemSpriteExtra* create(cocos2d::CCNode* sprite, cocos2d::CCNode* selectedSprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);

    CCMenuItemSpriteExtra();
    void /* TodoReturn */ applyLargeMobileTouch();
    bool init(cocos2d::CCNode* sprite, cocos2d::CCNode* selectedSprite, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
    void setClickColor(cocos2d::ccColor3B);
    void setSizeMult(float mult);
    void setSprite(cocos2d::CCSprite* sprite);
    void updateSprite();
    void useAnimationType(MenuAnimationType type);

    float m_scaleMultiplier;
    float m_baseScale;
    bool m_animationEnabled;
    bool m_colorEnabled;
    float m_volume;
    std::string m_activateSound;
    std::string m_selectSound;
    float m_colorDip;
    cocos2d::CCPoint m_offset;
    float m_duration;
    float m_unselectedDuration;
    MenuAnimationType m_animationType;
    cocos2d::CCPoint m_startPosition;
    IconType m_iconType;
    cocos2d::ccColor3B m_clickColor;

};
