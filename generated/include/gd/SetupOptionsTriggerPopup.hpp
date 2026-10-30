#pragma once

class GameOptionsTrigger;
class cocos2d;

class SetupOptionsTriggerPopup : public SetupTriggerPopup {
public:
    virtual ~SetupOptionsTriggerPopup();
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SetupOptionsTriggerPopup* create(GameOptionsTrigger* object, cocos2d::CCArray* objects);

    SetupOptionsTriggerPopup();
    void addOption(int property, std::string label);
    bool init(GameOptionsTrigger* object, cocos2d::CCArray* objects);

    cocos2d::CCPoint m_togglePosition;
    int m_togglePage;

};
