#pragma once

class CCMenuItemSpriteExtra;
class cocos2d;

class CCMenuItemToggler : public cocos2d::CCMenuItem {
public:
    virtual ~CCMenuItemToggler();
    virtual void activate();
    virtual void selected();
    virtual void unselected();
    virtual void setEnabled(bool enabled);

    static CCMenuItemToggler* create(cocos2d::CCNode* off, cocos2d::CCNode* on, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
    static CCMenuItemToggler* createWithSize(const char* spr1, const char* spr2, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale);
    static CCMenuItemToggler* createWithStandardSprites(cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback, float scale);

    CCMenuItemToggler();
    CCMenuItemSpriteExtra* activeItem();
    bool init(cocos2d::CCNode* off, cocos2d::CCNode* on, cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback);
    bool isOn();
    bool isToggled();
    void normalTouch(cocos2d::CCObject* sender);
    void selectedTouch(cocos2d::CCObject* sender);
    void setClickable(bool on);
    void setSizeMult(float mult);
    void setSpriteColor(cocos2d::ccColor3B const&);
    void toggle(bool on);
    void toggleWithCallback(bool on);
    void updateSprite();

    CCMenuItemSpriteExtra* m_offButton;
    CCMenuItemSpriteExtra* m_onButton;
    bool m_toggled;
    bool m_notClickable;

};
