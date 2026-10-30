#pragma once

class GJBaseGameLayer;

class GradientTriggerObject : public EffectGameObject {
public:
    virtual ~GradientTriggerObject();
    virtual bool init();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static GradientTriggerObject* create();

    GradientTriggerObject();

    int m_blendingLayer;
    int m_blendingMode;
    int m_gradientID;
    int m_upBottomLeftID;
    int m_downBottomRightID;
    int m_leftTopLeftID;
    int m_rightTopRightID;
    bool m_vertexMode;
    bool m_disable;
    bool m_disableAll;
    float m_previewOpacity;

};
