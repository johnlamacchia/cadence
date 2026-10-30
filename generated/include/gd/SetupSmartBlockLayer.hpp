#pragma once

class GameObject;
class SelectArtLayer;
class SmartGameObject;
class cocos2d;

class SetupSmartBlockLayer : public FLAlertLayer, public SelectArtDelegate {
public:
    virtual ~SetupSmartBlockLayer();
    virtual void keyBackClicked();
    virtual void show();
    virtual void selectArtClosed(SelectArtLayer* layer);

    static SetupSmartBlockLayer* create(SmartGameObject* object, cocos2d::CCArray* objects);

    void determineStartValues();
    bool init(SmartGameObject* object, cocos2d::CCArray* objects);
    void onAllowFlipping(cocos2d::CCObject* sender);
    void onAllowRotation(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onCreate(cocos2d::CCObject* sender);
    void onCreateAll(cocos2d::CCObject* sender);
    void onCreateTemplate(cocos2d::CCObject* sender);
    void onDontDelete(cocos2d::CCObject* sender);
    void onIgnoreCorners(cocos2d::CCObject* sender);
    void onNearbyReference(cocos2d::CCObject* sender);
    void onPasteTemplate(cocos2d::CCObject* sender);
    void onReferenceOnly(cocos2d::CCObject* sender);
    void onSelectPremade(cocos2d::CCObject* sender);
    void onSelectTemplate(cocos2d::CCObject* sender);

    bool m_referenceOnly;
    bool m_allowRotation;
    bool m_allowFlipX;
    bool m_allowFlipY;
    bool m_useNearby;
    bool m_dontDelete;
    bool m_ignoreCorners;
    int m_specialTemplate;
    SmartGameObject* m_gameObject;
    cocos2d::CCArray* m_gameObjects;

};
