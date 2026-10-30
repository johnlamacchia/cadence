#pragma once

class GJBaseGameLayer;
class GameObject;

class UISettingsGameObject : public EffectGameObject {
public:
    virtual ~UISettingsGameObject();
    virtual bool init();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static UISettingsGameObject* create();

    UISettingsGameObject();

    int m_xRef;
    int m_yRef;
    bool m_xRelative;
    bool m_yRelative;

};
