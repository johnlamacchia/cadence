#pragma once

class GJBaseGameLayer;
class GameObject;

class ShaderGameObject : public EffectGameObject {
public:
    virtual ~ShaderGameObject();
    virtual void customSetup();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static ShaderGameObject* create(char const* frame);

    ShaderGameObject();
    bool init(char const* frame);

    float m_speed;
    float m_strength;
    float m_outer;
    float m_timeOff;
    float m_waveWidth;
    float m_targetX;
    float m_targetY;
    float m_fadeIn;
    float m_fadeOut;
    float m_screenOffsetX;
    float m_screenOffsetY;
    bool m_invert;
    float m_inner;
    float m_maxSize;
    bool m_flip;
    bool m_rotate;
    bool m_dual;
    bool m_useX;
    bool m_useY;
    bool m_snapGrid;
    bool m_hardEdges;
    bool m_disableAll;
    int m_zLayerMin;
    int m_zLayerMax;
    bool m_animate;
    bool m_relative;
    bool m_editorDisabled;

};
