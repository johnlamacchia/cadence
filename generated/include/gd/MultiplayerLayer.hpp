#pragma once

class cocos2d;

class MultiplayerLayer : public cocos2d::CCLayer {
public:
    virtual bool init();
    virtual void keyBackClicked();

    static MultiplayerLayer* create();
    static cocos2d::CCScene* scene();

    void onBack(cocos2d::CCObject* sender);
    void onBtn1(cocos2d::CCObject* sender);
    void onBtn2(cocos2d::CCObject* sender);

};
