#pragma once

class cocos2d;

class HardStreak : public cocos2d::CCDrawNode {
public:
    virtual ~HardStreak();
    virtual bool init();

    static HardStreak* create();

    void addPoint(cocos2d::CCPoint point);
    void appendQuad(cocos2d::CCPoint const* vertices, cocos2d::ccColor4F const& color);
    void clearAboveXPos(float x);
    void clearBehindXPos(float x);
    HardStreak* createDuplicate();
    void /* TodoReturn */ ensureQuadCapacity(unsigned int);
    void firstSetup();
    double normalizeAngle(double angle);
    cocos2d::CCPoint quadCornerOffset(cocos2d::CCPoint start, cocos2d::CCPoint end, float width);
    void reset();
    void resumeStroke();
    void scheduleAutoUpdate();
    void stopStroke();
    void updateStroke(float dt);

    cocos2d::CCArray* m_pointArray;
    cocos2d::CCPoint m_currentPoint;
    float m_waveSize;
    float m_pulseSize;
    bool m_isSolid;
    bool m_isFlipped;
    bool m_drawStreak;

};
