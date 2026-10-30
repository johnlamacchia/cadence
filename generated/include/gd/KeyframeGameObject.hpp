#pragma once

class EditorUI;
class GJBaseGameLayer;
class GameObject;
class cocos2d;

class KeyframeGameObject : public EffectGameObject {
public:
    virtual bool init();
    virtual void setOpacity(unsigned char opacity);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static KeyframeGameObject* create();

    KeyframeGameObject();
    ~KeyframeGameObject();
    void updateShadowObjects(GJBaseGameLayer* layer, EditorUI* ui);

    cocos2d::CCArray* m_shadowObjects;
    cocos2d::CCSprite* m_previewSprite;
    int m_keyframeGroup;
    int m_keyframeIndex;
    bool m_referenceOnly;
    bool m_proximity;
    bool m_curve;
    bool m_closeLoop;
    int m_timeMode;
    float m_unk760;
    float m_spawnDelay;
    bool m_previewArt;
    bool m_keyframeActive;
    bool m_autoLayer;
    int m_direction;
    int m_revolutions;
    float m_lineOpacity;

};
