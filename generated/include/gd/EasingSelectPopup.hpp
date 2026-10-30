#pragma once

class cocos2d;

class EasingSelectPopup : public SetupTriggerPopup {
public:
    virtual ~EasingSelectPopup();
    virtual void update(float dt);
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void valueDidChange(int, float);

    static EasingSelectPopup* create(SetupTriggerPopup*, int, int, int);

    bool init(SetupTriggerPopup*, int, int, int);
    void onCell(cocos2d::CCObject* sender);
    void onRestart(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshCellHighlight();
    void /* TodoReturn */ refreshRateVisual();

};
