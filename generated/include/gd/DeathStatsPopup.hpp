#pragma once

class LevelEditorLayer;
class cocos2d;

class DeathStatsPopup : public SetupTriggerPopup {
public:
    virtual ~DeathStatsPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);

    static DeathStatsPopup* create(LevelEditorLayer*);

    bool init(LevelEditorLayer*);
    void onClearFetched(cocos2d::CCObject* sender);
    void onClearLocal(cocos2d::CCObject* sender);
    void onFetch(cocos2d::CCObject* sender);
    void /* TodoReturn */ pollFetch(float);
    void setStatus(char const*);
    void /* TodoReturn */ showResult();

};
