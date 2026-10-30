#pragma once

class InfoLayer;
class cocos2d;

class NewgroundsInfoLayer : public FLAlertLayer {
public:
    virtual ~NewgroundsInfoLayer();
    virtual bool init();
    virtual void keyBackClicked();

    static NewgroundsInfoLayer* create();

    NewgroundsInfoLayer();
    void onArtists(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onGuidelines(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onNewgrounds(cocos2d::CCObject* sender);
    void onSupporter(cocos2d::CCObject* sender);

};
