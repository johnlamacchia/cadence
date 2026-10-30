#pragma once

class Slider;
class cocos2d;

class SliderThumb : public cocos2d::CCMenuItemImage {
public:
    virtual ~SliderThumb();

    static SliderThumb* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler selector, char const* normalFrame, char const* selectedFrame);

    SliderThumb();
    float getRawValue();
    float getValue();
    bool init(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler selector, char const* normalFrame, char const* selectedFrame);
    void setMaxOffset(float offset);
    void setRotated(bool rotated);
    void setSnap(float, float);
    void setSnapStep(float);
    void setValue(float value);

    float m_length;
    bool m_vertical;
    float m_snapValue;
    float m_snapThreshold;
    float m_snapStep;

};
