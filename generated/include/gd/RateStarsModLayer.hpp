#pragma once

class UploadActionPopup;
class cocos2d;

class RateStarsModLayer : public FLAlertLayer, public UploadPopupDelegate, public UploadActionDelegate {
public:
    virtual ~RateStarsModLayer();
    virtual void keyBackClicked();
    virtual void uploadActionFinished(int, int);
    virtual void uploadActionFailed(int, int);
    virtual void onClosePopup(UploadActionPopup*);

    static RateStarsModLayer* create(int, bool);

    void /* TodoReturn */ getStarsButton(int, cocos2d::SEL_MenuHandler, cocos2d::CCMenu*, float);
    bool init(int, bool);
    void onClose(cocos2d::CCObject* sender);
    void onFeature(cocos2d::CCObject* sender);
    void onRate(cocos2d::CCObject* sender);
    void onToggleCoins(cocos2d::CCObject* sender);
    void /* TodoReturn */ selectDemon(cocos2d::CCObject*);
    void /* TodoReturn */ selectRating(cocos2d::CCObject*);
    void /* TodoReturn */ updateDemonRow();
    void /* TodoReturn */ updateDSprite();

};
