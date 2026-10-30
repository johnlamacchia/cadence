#pragma once

class GJBaseGameLayer;
class GameObject;

class SongTriggerGameObject : public SFXTriggerGameObject {
public:
    virtual ~SongTriggerGameObject();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static SongTriggerGameObject* create(char const* frame);

    SongTriggerGameObject();
    bool init(char const* frame);

    bool m_unk7a9;
    bool m_prep;
    bool m_loadPrep;
    int m_songChannel;

};
