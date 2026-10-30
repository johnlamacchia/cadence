#pragma once

class GJBaseGameLayer;
class PlayerObject;

class EnhancedGameObject : public GameObject {
public:
    virtual ~EnhancedGameObject();
    virtual void customSetup();
    virtual void resetObject();
    virtual void deactivateObject(bool deactivate);
    virtual void customObjectSetup(std::vector<std::string>& values, std::vector<void*>& exists);
    virtual std::string getSaveString(GJBaseGameLayer* layer);
    virtual void triggerActivated(float xPosition);
    virtual void restoreObject();
    virtual void animationTriggered();
    virtual void activatedByPlayer(PlayerObject* player);
    virtual bool hasBeenActivatedByPlayer(PlayerObject* player);
    virtual bool hasBeenActivated();
    virtual void saveActiveColors();
    virtual bool canAllowMultiActivate();
    virtual bool getHasSyncedAnimation();
    virtual bool getHasRotateAction();
    virtual bool canMultiActivate(bool multiActivate);
    virtual void powerOnObject(int state);
    virtual void powerOffObject();
    virtual void stateSensitiveOff(GJBaseGameLayer* layer);
    virtual void updateSyncedAnimation(float totalTime, int frameIndex);
    virtual void updateAnimateOnTrigger(bool animate);

    static EnhancedGameObject* create(char const* frame);

    EnhancedGameObject();
    void createRotateAction(float angle, int clockwiseDirection);
    bool init(char const* frame);
    void previewAnimateOnTrigger();
    void refreshRotateAction();
    void resetSyncedAnimation();
    void setupAnimationVariables();
    void triggerAnimation();
    void updateRotateAction(float dt);
    void updateState(int state);
    void updateUserCoin();
    void waitForAnimationTrigger();

    bool m_poweredOn;
    int m_state;
    int m_animationRandomizedStartValue;
    float m_animationStart;
    float m_unk540;
    float m_unk544;
    bool m_unk548;
    float m_randomFrameTime;
    bool m_visible;
    bool m_shouldNotHideAnimFreeze;
    bool m_usesSpecialAnimation;
    float m_frameTime;
    short m_frames;
    bool m_hasCustomAnimation;
    bool m_hasCustomRotation;
    bool m_disableRotation;
    float m_rotationSpeed;
    float m_rotationAngle;
    float m_rotationDelta;
    float m_rotationAnimationSpeed;
    bool m_animationRandomizedStart;
    float m_animationSpeed;
    bool m_animationShouldUseSpeed;
    bool m_animateOnTrigger;
    bool m_disableDelayedLoop;
    bool m_disableAnimShine;
    int m_singleFrame;
    bool m_animationOffset;
    bool m_animationTriggered;
    int m_unkAnimationInt;
    int m_maybeAnimationVariableXInt;
    int m_maybeAnimationVariableYInt;
    bool m_animateOnlyWhenActive;
    bool m_isNoMultiActivate;
    bool m_isMultiActivate;
    bool m_activated;
    bool m_activatedByPlayer1;
    bool m_activatedByPlayer2;
    bool m_hasUniqueCoin;

};
