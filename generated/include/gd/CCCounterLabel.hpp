#pragma once

class CCCounterLabel : public cocos2d::CCLabelBMFont {
public:
    virtual ~CCCounterLabel();

    static CCCounterLabel* create(int count, char const* font, FormatterType type);

    CCCounterLabel();
    void calculateStepSize(int steps);
    void disableCounter();
    void enableCounter();
    void fastUpdateCounter();
    int getTargetCount();
    bool init(int count, char const* font, FormatterType type);
    void setTargetCount(int count);
    void updateCounter(float dt);
    void updateString();

    int m_stepSize;
    int m_targetCount;
    int m_currentCount;
    bool m_counterEnabled;
    int m_stepCount;
    bool m_dontSchedule;
    FormatterType m_formatterType;

};
