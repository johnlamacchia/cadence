#pragma once

class cocos2d;

class GJPathsLayer : public FLAlertLayer, public FLAlertLayerProtocol {
public:
    virtual ~GJPathsLayer();
    virtual bool init();
    virtual void onExit();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();

    static GJPathsLayer* create();
    static std::string nameForPath(int path);

    void darkenButtons(bool darken);
    void onClose(cocos2d::CCObject* sender);
    void onPath(cocos2d::CCObject* sender);

    cocos2d::CCSprite* m_closeSprite;
    bool m_exiting;

};
