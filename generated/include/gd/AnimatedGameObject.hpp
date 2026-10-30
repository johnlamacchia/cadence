#pragma once

class CCAnimatedSprite;
class CCSpritePart;
class GameObject;
class cocos2d;

class AnimatedGameObject : public EnhancedGameObject, public AnimatedSpriteDelegate, public SpritePartDelegate {
public:
    virtual void setOpacity(unsigned char opacity);
    virtual void setChildColor(cocos2d::ccColor3B const& color);
    virtual void resetObject();
    virtual void activateObject();
    virtual void deactivateObject(bool deactivate);
    virtual void setObjectColor(cocos2d::ccColor3B const& color);
    virtual void animationFinished(char const* key);
    virtual void displayFrameChanged(cocos2d::CCObject* sprite, std::string frameName);

    static std::string animationForID(int type, int id);
    static AnimatedGameObject* create(int id);
    static float getTweenTime(int type, int id);

    AnimatedGameObject();
    ~AnimatedGameObject();
    bool init(int id);
    void playAnimation(int type);
    void setupAnimatedSize(int id);
    void setupChildSprites();
    void updateChildSpriteColor(cocos2d::ccColor3B color);
    void updateObjectAnimation();

    CCAnimatedSprite* m_animatedSprite;
    cocos2d::CCSprite* m_childSprite;
    CCSpritePart* m_eyeSpritePart;
    bool m_finishedAnimating;
    bool m_playingAnimation;
    std::string m_currentAnimation;
    bool m_notGrounded;
    int m_animationID;

};
