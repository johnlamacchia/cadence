#pragma once

class GJPathsLayer;
class GJStoreItem;
class cocos2d;

class GJPathPage : public FLAlertLayer, public FLAlertLayerProtocol, public GJPurchaseDelegate {
public:
    virtual ~GJPathPage();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void show();
    virtual void didPurchaseItem(GJStoreItem* item);

    static GJPathPage* create(int path, GJPathsLayer* layer);

    bool init(int path, GJPathsLayer* layer);
    void onActivatePath(cocos2d::CCObject* sender);
    void onBack(cocos2d::CCObject* sender);
    void onIconInfo(cocos2d::CCObject* sender);
    void onUnlock(cocos2d::CCObject* sender);
    void playUnlockAnimation();
    void showCantAffordMessage(GJStoreItem* item);
    void unlockAnimationFinished();
    void unlockAnimationStep2();
    void unlockAnimationStep3();

    GJPathsLayer* m_pathsLayer;
    int m_pathNumber;
    bool m_animationPlaying;
    int m_dialogIndex;

};
