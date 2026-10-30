#pragma once

class LikeItemDelegate;
class cocos2d;

class LikeItemLayer : public FLAlertLayer {
public:
    virtual ~LikeItemLayer();
    virtual void keyBackClicked();

    static LikeItemLayer* create(LikeItemType type, int id, int parentID);

    LikeItemLayer();
    bool init(LikeItemType type, int id, int parentID);
    void onClose(cocos2d::CCObject* sender);
    void onDislike(cocos2d::CCObject* sender);
    void onLike(cocos2d::CCObject* sender);
    void triggerLike(bool isLiked);

    LikeItemType m_itemType;
    int m_itemID;
    int m_commentSourceID;
    LikeItemDelegate* m_likeDelegate;

};
