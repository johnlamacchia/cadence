#pragma once

class AnimatedShopKeeper;
class CCCounterLabel;
class DialogLayer;
class GJStoreItem;
class cocos2d;

class GJShopLayer : public cocos2d::CCLayer, public GJPurchaseDelegate, public DialogDelegate, public RewardedVideoDelegate {
public:
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void didPurchaseItem(GJStoreItem* item);
    virtual void rewardedVideoFinished();
    virtual void dialogClosed(DialogLayer* layer);

    static GJShopLayer* create(ShopType type);
    static cocos2d::CCScene* scene(ShopType type);

    GJShopLayer();
    ~GJShopLayer();
    void exitVideoAdItems();
    bool init(ShopType type);
    void onBack(cocos2d::CCObject* sender);
    void onCommunityCredits(cocos2d::CCObject* sender);
    void onPlushies(cocos2d::CCObject* sender);
    void onSelectItem(cocos2d::CCObject* sender);
    void onVideoAd(cocos2d::CCObject* sender);
    void showCantAffordMessage(GJStoreItem* item);
    void showReactMessage();
    void updateCurrencyCounter();

    bool m_closing;
    CCCounterLabel* m_currencyLabel;
    cocos2d::CCDictionary* m_shopItems;
    ShopType m_type;
    std::string m_sheetName;
    cocos2d::CCNode* m_unkNode1;
    cocos2d::CCNode* m_unkNode2;
    bool m_videoPlaying;
    bool m_unkBool;
    AnimatedShopKeeper* m_shopKeeper;
    int m_zolgurothDialogIndex;
    int m_affordDialogIndex;

};
