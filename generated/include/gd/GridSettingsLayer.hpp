#pragma once

class EditorUI;
class cocos2d;

class GridSettingsLayer : public EditorWindowPopup {
public:
    virtual ~GridSettingsLayer();
    virtual void valueDidChange(int, float);
    virtual float getValue(int);
    virtual void onBuildWindowContent();

    static GridSettingsLayer* create(EditorUI*);

    bool init(EditorUI*);
    void onCycleDown(cocos2d::CCObject* sender);
    void onCycleUp(cocos2d::CCObject* sender);
    void onReset(cocos2d::CCObject* sender);
    void /* TodoReturn */ refreshValues();

};
