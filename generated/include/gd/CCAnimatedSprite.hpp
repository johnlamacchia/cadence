#pragma once

class AnimatedSpriteDelegate;
class CCPartAnimSprite;
class SpriteAnimationManager;
class cocos2d;

class CCAnimatedSprite : public cocos2d::CCSprite {
public:
    virtual void setOpacity(unsigned char opacity);
    virtual void setColor(cocos2d::ccColor3B const& color);
    virtual void animationFinished(char const* key);
    virtual void animationFinishedO(cocos2d::CCObject* str);

    static CCAnimatedSprite* createWithType(char const* definition, cocos2d::CCTexture2D* texture, bool useTexture);

    CCAnimatedSprite();
    ~CCAnimatedSprite();
    void cleanupSprite();
    bool initWithType(char const* definition, cocos2d::CCTexture2D* texture, bool useTexture);
    void loadType(char const* definition, cocos2d::CCTexture2D* texture, bool useTexture);
    void runAnimation(std::string animation);
    void runAnimationForced(std::string animation);
    void stopTween();
    void switchToMode(spriteMode mode);
    void tweenToAnimation(std::string animation, float duration);
    void tweenToAnimationFinished();
    void willPlayAnimation();

    std::string m_unkString1;
    std::string m_activeTween;
    SpriteAnimationManager* m_animationManager;
    cocos2d::CCSprite* m_sprite;
    cocos2d::CCSprite* m_fbfSprite;
    CCPartAnimSprite* m_paSprite;
    spriteMode m_spriteMode;
    std::string m_currentAnim;
    AnimatedSpriteDelegate* m_delegate;

};
