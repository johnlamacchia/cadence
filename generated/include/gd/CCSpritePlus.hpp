#pragma once

class cocos2d;

class CCSpritePlus : public cocos2d::CCSprite {
public:
    virtual void setScaleX(float scaleX);
    virtual void setScaleY(float scaleY);
    virtual void setScale(float scale);
    virtual void setPosition(cocos2d::CCPoint const& position);
    virtual void setRotation(float rotation);
    virtual void setRotationX(float rotationX);
    virtual void setRotationY(float rotationY);
    virtual bool initWithTexture(cocos2d::CCTexture2D* texture);
    virtual bool initWithSpriteFrameName(char const* frameName);
    virtual void setFlipX(bool flipX);
    virtual void setFlipY(bool flipY);

    static CCSpritePlus* createWithSpriteFrame(cocos2d::CCSpriteFrame* frame);
    static CCSpritePlus* createWithSpriteFrameName(char const* frame);

    CCSpritePlus();
    ~CCSpritePlus();
    void addFollower(cocos2d::CCNode* follower);
    void followSprite(CCSpritePlus* sprite);
    CCSpritePlus* getFollower();
    void removeFollower(cocos2d::CCNode* sprite);
    void stopFollow();

    cocos2d::CCArray* m_followers;
    CCSpritePlus* m_followingSprite;
    bool m_hasFollower;
    bool m_propagateScaleChanges;
    bool m_propagateFlipChanges;

};
