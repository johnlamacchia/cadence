#pragma once

class CCMenuItemSpriteExtra;
class cocos2d;

class TutorialLayer : public FLAlertLayer {
public:
    virtual ~TutorialLayer();
    virtual bool init();
    virtual void keyBackClicked();

    static TutorialLayer* create();

    void loadPage(int page);
    void onClose(cocos2d::CCObject* sender);
    void onNext(cocos2d::CCObject* sender);
    void removeTutorialTexture();

    cocos2d::CCLayer* m_tutorialLayer;
    int m_page;
    CCMenuItemSpriteExtra* m_nextButton;
    CCMenuItemSpriteExtra* m_exitButton;

};
