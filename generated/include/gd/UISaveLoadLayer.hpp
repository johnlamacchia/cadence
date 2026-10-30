#pragma once

class OptionsLayer;
class UIOptionsLayer;
class cocos2d;

class UISaveLoadLayer : public SetupTriggerPopup {
public:
    static UISaveLoadLayer* create(UIOptionsLayer* layer);

    UISaveLoadLayer();
    bool init(UIOptionsLayer* layer);
    void onLoad(cocos2d::CCObject* sender);
    void onSave(cocos2d::CCObject* sender);

    UIOptionsLayer* m_optionsLayer;

};
