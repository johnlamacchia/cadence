#pragma once

class GJBaseGameLayer;
class cocos2d;

class EnterEffectObject : public EffectGameObject {
public:
    virtual ~EnterEffectObject();
    virtual void customSetup();
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);

    static EnterEffectObject* create(char const* frame);

    EnterEffectObject();
    bool init(char const* frame);
    void resetEnterAnimValues();

    int m_enterType;
    int m_length;
    int m_lengthVariance;
    int m_offset;
    int m_offsetVariance;
    int m_offsetY;
    int m_offsetYVariance;
    int m_moveDistance;
    int m_moveDistanceVariance;
    float m_areaScaleX;
    float m_areaScaleXVariance;
    float m_areaScaleY;
    float m_areaScaleYVariance;
    int m_moveAngle;
    int m_moveAngleVariance;
    bool m_startAngle;
    cocos2d::CCPoint m_anglePosition;
    bool m_relative;
    float m_relativeFade;
    EasingType m_easingInType;
    float m_easingInRate;
    int m_easingInBuffer;
    EasingType m_easingOutType;
    float m_easingOutRate;
    int m_easingOutBuffer;
    int m_moveX;
    int m_moveXVariance;
    int m_moveY;
    int m_moveYVariance;
    int m_tintChannelID;
    int m_property224;
    int m_directionType;
    bool m_xyMode;
    bool m_easeOutEnabled;
    float m_modFront;
    float m_modBack;
    float m_areaTint;
    float m_property285;
    int m_effectID;
    float m_areaRotation;
    float m_areaRotationVariance;
    float m_toOpacity;
    float m_fromOpacity;
    bool m_inbound;
    bool m_hsvEnabled;
    float m_deadzone;
    bool m_twoDirections;
    bool m_dontEditAreaParent;
    int m_priority;
    int m_unk7d8;
    int m_enterChannel;
    bool m_useEffectID;
    cocos2d::CCPoint m_unk7e4;
    cocos2d::CCPoint m_unk7ec;
    bool m_negativeTargetX;
    int m_areaRange;
    int m_unk7fc;

};
