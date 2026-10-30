#pragma once

class cocos2d;

class ItemInfoPopup : public FLAlertLayer {
public:
    virtual ~ItemInfoPopup();
    virtual void keyBackClicked();

    static ItemInfoPopup* create(int id, UnlockType type);
    static std::string nameForUnlockType(int id, UnlockType type);

    bool init(int id, UnlockType type);
    bool isUnlockedByDefault(int id, UnlockType type);
    void onClose(cocos2d::CCObject* sender);
    void onCredit(cocos2d::CCObject* sender);

    int m_itemID;
    UnlockType m_unlockType;
    int m_accountID;

};
