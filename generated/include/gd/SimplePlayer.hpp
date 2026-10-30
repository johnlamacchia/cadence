#pragma once

class GJRobotSprite;
class GJSpiderSprite;
class cocos2d;

class SimplePlayer : public cocos2d::CCSprite {
public:
    virtual void setOpacity(unsigned char opacity);
    virtual void setColor(cocos2d::ccColor3B const& color);

    static SimplePlayer* create(int id);

    SimplePlayer();
    ~SimplePlayer();
    void asyncLoadIcon(int id, IconType type);
    void createRobotSprite(int frame);
    void createSpiderSprite(int frame);
    void disableCustomGlowColor();
    void disableGlowOutline();
    void enableCustomGlowColor(cocos2d::ccColor3B const& color);
    void hideAll();
    void hideSecondary();
    void iconFinishedLoading(int id, IconType type);
    bool init(int id);
    void setColors(cocos2d::ccColor3B const& color1, cocos2d::ccColor3B const& color2);
    void setFrames(char const* firstFrame, char const* secondFrame, char const* ufoFrame, char const* glowFrame, char const* extraFrame);
    void setGlowOutline(cocos2d::ccColor3B color);
    void setSecondColor(cocos2d::ccColor3B const& color);
    void tryEnableCustomGlowColor(int id);
    void updateColors();
    void updatePlayerFrame(int id, IconType type);

    cocos2d::CCSprite* m_firstLayer;
    cocos2d::CCSprite* m_secondLayer;
    cocos2d::CCSprite* m_birdDome;
    cocos2d::CCSprite* m_outlineSprite;
    cocos2d::CCSprite* m_detailSprite;
    GJRobotSprite* m_robotSprite;
    GJSpiderSprite* m_spiderSprite;
    int m_unknown;
    bool m_hasGlowOutline;
    int m_iconRequestID;
    bool m_hasCustomGlowColor;
    cocos2d::ccColor3B m_glowColor;
    bool m_iconLoaded;

};
