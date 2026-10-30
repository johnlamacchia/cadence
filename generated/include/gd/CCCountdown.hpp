#pragma once

class cocos2d;

class CCCountdown : public cocos2d::CCSprite {
public:
    virtual bool init();
    virtual void setOpacity(unsigned char opacity);

    static CCCountdown* create();

    void lapFinished();
    void startTimerWithSeconds(float seconds, cocos2d::SEL_CallFunc selector, cocos2d::CCNode* target);

    cocos2d::CCProgressTimer* m_progressTimer;
    float m_unk260;
    int m_seconds;
    cocos2d::CCLabelBMFont* m_countdownLabel;
    cocos2d::SEL_CallFunc m_callbackSelector;
    cocos2d::CCNode* m_targetNode;

};
