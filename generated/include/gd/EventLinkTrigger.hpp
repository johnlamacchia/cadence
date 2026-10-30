#pragma once

class GJBaseGameLayer;

class EventLinkTrigger : public EffectGameObject {
public:
    virtual ~EventLinkTrigger();
    virtual bool init();
    virtual void triggerObject(GJBaseGameLayer* layer, int uniqueID, std::vector<int> const* remapKeys);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static EventLinkTrigger* create();

    EventLinkTrigger();

    std::set<int> m_eventIDs;
    bool m_resetRemap;
    int m_extraID;
    int m_extraID2;

};
