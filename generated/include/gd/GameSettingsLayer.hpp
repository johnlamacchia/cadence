#pragma once

class GJBaseGameLayer;
class GJDropDownLayer;
class cocos2d;

class GameSettingsLayer : public TabbedPopupLayer, public GJDropDownLayerDelegate {
public:
    virtual ~GameSettingsLayer();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void onCustomButton(cocos2d::CCObject* sender);
    virtual void valueDidChange(int, float);
    virtual float getValue(int);
    virtual void showTab(int tab);
    virtual void dropDownLayerWillClose(GJDropDownLayer*);

    static GameSettingsLayer* create(GJBaseGameLayer*, int);

    void addGVToggle(char const* name, char const* gameVar, char const* desc);
    void addSpecialContent();
    void addToggle(char const* name, int id, char const* desc, bool disabled);
    void buildTabContent();
    void /* TodoReturn */ didChangeGV(char const*, bool);
    bool init(GJBaseGameLayer*, int);
    void /* TodoReturn */ nextPosition(int);
    void onButtonRows(cocos2d::CCObject* sender);
    void onButtonsPerRow(cocos2d::CCObject* sender);
    void setupTabs();
    void /* TodoReturn */ showPracticeMusicSyncUnlockInfo();
    void /* TodoReturn */ toggleVisibility(bool);

};
