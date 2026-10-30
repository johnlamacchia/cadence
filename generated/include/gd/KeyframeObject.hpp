#pragma once

class cocos2d;
class tk_spline;

class KeyframeObject {
public:
    void setupSpline(std::vector<KeyframeObject*>& objects);

    double m_duration;
    int m_easingType;
    float m_easingRate;
    bool m_refOnly;
    int m_spawnGroup;
    float m_spawnDelay;
    bool m_proximity;
    bool m_spawned;
    bool m_curve;
    tk_spline m_spline1;
    tk_spline m_spline2;
    double m_curveLength;
    double m_refRunDuration;
    int m_timeMode;
    int m_keyframeObject;
    cocos2d::CCPoint m_positionDelta;
    cocos2d::CCPoint m_position;
    double m_scaleX;
    double m_scaleY;
    double m_rotation;
    double m_unk1b0;
    float m_opacity;

};
