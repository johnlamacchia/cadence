#pragma once

class GJPurchaseDelegate;
class GJStoreItem;
class cocos2d;

class PurchaseItemPopup : public FLAlertLayer {
public:
    virtual ~PurchaseItemPopup();
    virtual void keyBackClicked();

    static PurchaseItemPopup* create(GJStoreItem* item);

    bool init(GJStoreItem* item);
    void onClose(cocos2d::CCObject* sender);
    void onPurchase(cocos2d::CCObject* sender);

    GJStoreItem* m_storeItem;
    GJPurchaseDelegate* m_delegate;

};
