#pragma once

class SpritePartDelegate;
class cocos2d;

class CCSpritePart : public CCSpritePlus {
public:
    virtual ~CCSpritePart();
    virtual void setVisible(bool visible);

    static CCSpritePart* create(cocos2d::CCTexture2D* texture);
    static CCSpritePart* createWithSpriteFrameName(char const* frame);

    CCSpritePart();
    void frameChanged(std::string frame);
    bool getBeingUsed();
    void hideInactive();
    void markAsNotBeingUsed();
    void resetTextureRect();
    void setBeingUsed(bool beingUsed);
    void updateDisplayFrame(std::string frame);

    bool m_isBeingUsed;
    std::string m_spriteFrameName;
    SpritePartDelegate* m_delegate;

};
