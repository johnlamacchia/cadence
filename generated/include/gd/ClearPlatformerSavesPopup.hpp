#pragma once

class cocos2d;

class ClearPlatformerSavesPopup : public FLAlertLayer, public FLAlertLayerProtocol {
public:
    virtual ~ClearPlatformerSavesPopup();
    virtual bool init();
    virtual void keyBackClicked();
    virtual void FLAlert_Clicked(FLAlertLayer*, bool);

    static ClearPlatformerSavesPopup* create();

    void onClose(cocos2d::CCObject* sender);
    void onDelete(cocos2d::CCObject* sender);
    void onIncludeFavoritesToggle(cocos2d::CCObject* sender);
    void onOptions(cocos2d::CCObject* sender);
    void /* TodoReturn */ updateStats();

};
