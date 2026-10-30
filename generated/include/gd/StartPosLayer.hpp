#pragma once

class StartPosObject;
class cocos2d;

class StartPosLayer : public SetupTriggerPopup {
public:
    virtual ~StartPosLayer();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void updateInputNode(int, float);
    virtual void valueDidChange(int, float);
    virtual float getValue(int);

    static StartPosLayer* create(StartPosObject*);

    void addModeButtons();
    void addSpeedButtons();
    bool init(StartPosObject*);
    void onSelectMode(cocos2d::CCObject* sender);
    void onSelectSpeed(cocos2d::CCObject* sender);

    StartPosObject* m_startPosObject;

};
