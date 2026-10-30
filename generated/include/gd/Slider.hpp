#pragma once

class SliderDelegate;
class SliderThumb;
class SliderTouchLogic;
class cocos2d;

class Slider : public cocos2d::CCLayer {
public:
    virtual ~Slider();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler handler);
    static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler handler, float scale);
    static Slider* create(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler handler, char const* bar, char const* groove, char const* thumb, char const* thumbSel, float scale);

    void disableSlider();
    void disableTouch();
    void enableSlider();
    bool getLiveDragging();
    SliderThumb* getThumb();
    float getValue();
    void hideGroove(bool visibility);
    bool init(cocos2d::CCNode* target, cocos2d::SEL_MenuHandler handler, char const* bar, char const* groove, char const* thumb, char const* thumbSel, float scale);
    void setBarVisibility(bool visibility);
    void setLiveDragging(bool activateThumb);
    void setMaxOffset(float offset);
    void setRotated(bool rotated);
    void setSnap(float, float);
    void setSnapStep(float);
    void setSnapStepCalc(float, float, float);
    void setValue(float val);
    void sliderBegan();
    void sliderEnded();
    void updateBar();

    SliderTouchLogic* m_touchLogic;
    cocos2d::CCSprite* m_sliderBar;
    cocos2d::CCSprite* m_groove;
    float m_width;
    float m_height;
    SliderDelegate* m_delegate;
    bool m_enabled;

};
