#pragma once

class EnterEffectAnimValue;
class EnterEffectObject;
class GJBaseGameLayer;
class cocos2d;

class EnterEffectInstance {
public:
    EnterEffectInstance();
    EnterEffectInstance(EnterEffectObject* object, int targetID, int centerID, int unknown, int targetGroupIndex, int easeIndexCount, int controlID);
    void animateValue(int key, float value, float distance, float duration, int easingType, float easingRate, int easingBuffer);
    float getValue(int key);
    void loadTransitions(EnterEffectObject* object, float time);
    void loadValuesFromObject(EnterEffectObject* object);
    void setValue(int key, float value);
    void updateTransitions(float dt, GJBaseGameLayer* layer);

    std::map<int,EnterEffectAnimValue> m_enterEffectAnimMap;
    float m_length;
    float m_lengthVariance;
    float m_offset;
    float m_offsetVariance;
    float m_offsetY;
    float m_offsetYVariance;
    float m_modFront;
    float m_modBack;
    float m_deadzone;
    float m_moveDistance;
    float m_moveDistanceVariance;
    float m_moveAngle;
    float m_moveAngleVariance;
    float m_moveX;
    float m_moveXVariance;
    float m_moveY;
    float m_moveYVariance;
    float m_relativeFade;
    float m_scaleX;
    float m_scaleXVariance;
    float m_scaleY;
    float m_scaleYVariance;
    float m_rotation;
    float m_rotationVariance;
    float m_tint;
    float m_unusedKey285;
    float m_toOpacity;
    float m_fromOpacity;
    cocos2d::ccHSVValue m_hsvValue;
    float m_hue;
    float m_saturation;
    float m_value;
    EnterEffectObject* m_gameObject;
    bool m_reversed;
    int m_targetID;
    int m_centerID;
    int m_startOrder;
    int m_easeIndex;
    bool m_paused;
    bool m_paused2;
    bool m_unkBool4;
    int m_targetGroupIndex;
    std::vector<int> m_easeIndices;
    int m_controlID;

};
