#pragma once

class SetupEventLinkPopup;
class cocos2d;

class SelectEventLayer : public SetupTriggerPopup {
public:
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void onCustomToggleTriggerValue(cocos2d::CCObject* sender);

    static SelectEventLayer* create(SetupEventLinkPopup* popup, std::set<int>& eventIDs);

    SelectEventLayer();
    ~SelectEventLayer();
    void addToggle(int id, std::string info);
    bool init(SetupEventLinkPopup* popup, std::set<int>& eventIDs);
    cocos2d::CCPoint nextPosition();
    void onInfo(cocos2d::CCObject* sender);

    SetupEventLinkPopup* m_eventLinkPopup;
    bool m_eventsChanged;
    int m_unk3b4;
    int m_toggleCount;
    int m_maxPage;
    std::map<int, std::string> m_eventInfos;
    std::set<int> m_eventIDs;

};
