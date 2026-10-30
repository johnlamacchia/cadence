#pragma once

class SecretGame01Layer;
class SecretLayer;
class cocos2d;

class SecretLayer6 : public cocos2d::CCLayer {
public:
    virtual bool init();
    virtual void keyBackClicked();

    static SecretLayer6* create();
    static cocos2d::CCScene* scene();

    void onBack(cocos2d::CCObject* sender);
    void startGame01();

    SecretGame01Layer* m_gameLayer;

};
