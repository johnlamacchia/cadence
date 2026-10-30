#pragma once

class EventLinkTrigger;
class cocos2d;

class SetupEventLinkPopup : public SetupTriggerPopup {
public:
    virtual ~SetupEventLinkPopup();

    static SetupEventLinkPopup* create(EventLinkTrigger* object, cocos2d::CCArray* objects);

    bool init(EventLinkTrigger* object, cocos2d::CCArray* objects);
    void onSelectEvent(cocos2d::CCObject* sender);
    void updateEventIDs(std::set<int>& eventIDs);

};
