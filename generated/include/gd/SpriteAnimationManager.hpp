#pragma once

class CCAnimatedSprite;
class cocos2d;

class SpriteAnimationManager : public cocos2d::CCNode {
public:
    static void createAnimations(std::string definition);
    static SpriteAnimationManager* createWithOwner(CCAnimatedSprite* sprite, std::string definition);

    SpriteAnimationManager();
    ~SpriteAnimationManager();
    void animationFinished();
    void callAnimationFinished();
    void doCleanup();
    void executeAnimation(std::string animation);
    void finishAnimation(std::string animation);
    spriteMode getAnimType(std::string animation);
    int getPrio(std::string animation);
    bool initWithOwner(CCAnimatedSprite* sprite, std::string definition);
    void loadAnimations(std::string definition);
    void offsetCurrentAnimation(float dt);
    void overridePrio();
    void playSound(std::string sound);
    void playSoundForAnimation(std::string animation);
    void queueAnimation(std::string animation);
    void resetAnimState();
    void runAnimation(std::string animation);
    void runQueuedAnimation();
    void stopAnimations();
    void storeAnimation(cocos2d::CCAnimate* action, cocos2d::CCAnimate* frames, std::string name, int priority, spriteMode type, cocos2d::CCSpriteFrame* first);
    void storeSoundForAnimation(cocos2d::CCString* sound, std::string animation, float delay);
    void switchToFirstFrameOfAnimation(std::string animation);
    void updateAnimationSpeed(float speed);

    CCAnimatedSprite* m_sprite;
    cocos2d::CCDictionary* m_priorityDict;
    cocos2d::CCDictionary* m_typeDict;
    cocos2d::CCDictionary* m_soundDict;
    std::string m_queuedAnimation;
    float m_speed;
    cocos2d::CCDictionary* m_animateDict;
    cocos2d::CCDictionary* m_frameDict;
    std::string m_currentAnimation;
    std::string m_nextAnimation;

};
