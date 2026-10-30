#pragma once

class GameObject;
class cocos2d;

class SetupObjectOptionsPopup : public FLAlertLayer {
public:
    virtual void keyDown(cocos2d::enumKeyCodes, double);
    virtual void keyBackClicked();
    virtual void show();
    virtual void determineStartValues();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual bool handleAction(int, bool, double);

    static SetupObjectOptionsPopup* create(GameObject* object, cocos2d::CCArray* objects);

    SetupObjectOptionsPopup();
    ~SetupObjectOptionsPopup();
    bool init(GameObject* object, cocos2d::CCArray* objects);
    void onAlwaysHide(cocos2d::CCObject* sender);
    void onApplyScaleStick(cocos2d::CCObject* sender);
    void onCenterEffect(cocos2d::CCObject* sender);
    void onDisableGlow(cocos2d::CCObject* sender);
    void onDisableObject(cocos2d::CCObject* sender);
    void onDontBoost(cocos2d::CCObject* sender);
    void onDontBoostX(cocos2d::CCObject* sender);
    void onDontEnter(cocos2d::CCObject* sender);
    void onDontFade(cocos2d::CCObject* sender);
    void onExtendedCollision(cocos2d::CCObject* sender);
    void onExtraSticky(cocos2d::CCObject* sender);
    void onGripSlope(cocos2d::CCObject* sender);
    void onHideEffects(cocos2d::CCObject* sender);
    void onHighDetail(cocos2d::CCObject* sender);
    void onIceBlock(cocos2d::CCObject* sender);
    void onNoAudioScale(cocos2d::CCObject* sender);
    void onNonStickX(cocos2d::CCObject* sender);
    void onNonStickY(cocos2d::CCObject* sender);
    void onNoParticle(cocos2d::CCObject* sender);
    void onPassable(cocos2d::CCObject* sender);
    void onReverse(cocos2d::CCObject* sender);
    void onSinglePlayerTouch(cocos2d::CCObject* sender);
    void onToggleAreaParent(cocos2d::CCObject* sender);
    void onToggleGroupParent(cocos2d::CCObject* sender);

    GameObject* m_gameObject;
    cocos2d::CCArray* m_gameObjects;
    bool m_iceBlock;
    bool m_dontFade;
    bool m_dontEnter;
    bool m_noEffects;
    bool m_noParticle;
    bool m_noTouch;
    bool m_passable;
    bool m_hide;
    bool m_nonStickX;
    bool m_nonStickY;
    bool m_extraSticky;
    bool m_dontBoostY;
    bool m_dontBoostX;
    bool m_noAudioScale;
    bool m_scaleStick;
    bool m_highDetail;
    bool m_groupParent;
    bool m_areaParent;
    bool m_gripSlope;
    bool m_noGlow;
    bool m_reverse;
    bool m_extendedCollision;
    bool m_unk2a6;
    bool m_effectObject;
    bool m_unk2a7;
    bool m_singlePlayerTouch;
    bool m_centerEffect;

};
