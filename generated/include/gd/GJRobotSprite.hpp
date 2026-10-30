#pragma once

class CCSpritePart;
class cocos2d;

class GJRobotSprite : public CCAnimatedSprite {
public:
    virtual ~GJRobotSprite();
    virtual void setOpacity(unsigned char opacity);
    virtual void hideSecondary();

    static GJRobotSprite* create(int frame);

    void hideGlow();
    bool init(int frame);
    bool init(int frame, std::string animName);
    void showGlow();
    void updateColor01(cocos2d::ccColor3B color);
    void updateColor02(cocos2d::ccColor3B color);
    void updateColors();
    void updateFrame(int frame);
    void updateGlowColor(cocos2d::ccColor3B color, bool simple);

    cocos2d::CCArray* m_unkArray;
    bool m_hasExtra;
    cocos2d::ccColor3B m_color;
    cocos2d::ccColor3B m_secondColor;
    cocos2d::CCArray* m_secondArray;
    cocos2d::CCSprite* m_glowSprite;
    cocos2d::CCSprite* m_extraSprite;
    IconType m_iconType;
    int m_iconRequestID;
    CCSpritePart* m_headSprite;
    CCSpritePart* m_footSprite;

};
