#pragma once

class CCMenuItemSpriteExtra;
class cocos2d;

class TutorialPopup : public FLAlertLayer {
public:
    virtual void keyBackClicked();
    virtual void show();

    static TutorialPopup* create(std::string frame);

    void animateMenu();
    void closeTutorial(cocos2d::CCObject* sender);
    bool init(std::string frame);
    void registerForCallback(cocos2d::SEL_MenuHandler selector, cocos2d::CCNode* target);

    std::string m_currentSprite;
    bool m_callbackRegistered;
    cocos2d::SEL_MenuHandler m_callbackSelector;
    CCMenuItemSpriteExtra* m_closeButton;

};
