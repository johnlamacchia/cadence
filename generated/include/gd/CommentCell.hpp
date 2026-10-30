#pragma once

class CCMenuItemToggler;
class FLAlertLayer;
class GJComment;
class cocos2d;

class CommentCell : public TableViewCell, public LikeItemDelegate, public FLAlertLayerProtocol {
public:
    virtual bool init();
    virtual void draw();
    virtual void likedItem(LikeItemType type, int id, bool liked);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);

    CommentCell(char const* identifier, float width, float height);
    ~CommentCell();
    void incrementDislikes();
    void incrementLikes();
    void loadFromComment(GJComment* comment);
    void onBulkSelectToggle(cocos2d::CCObject* sender);
    void onConfirmDelete(cocos2d::CCObject* sender);
    void onDelete();
    void onGoToLevel(cocos2d::CCObject* sender);
    void onLike(cocos2d::CCObject* sender);
    void onMoreOptions(cocos2d::CCObject* sender);
    void onUndelete();
    void onUnhide(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void updateBGColor(int index);
    void /* TodoReturn */ updateBulkToggle();
    void updateLabelValues();

    cocos2d::CCSprite* m_iconSprite;
    cocos2d::CCLabelBMFont* m_likeLabel;
    GJComment* m_comment;
    bool m_accountComment;
    bool m_unk259;
    bool m_compactMode;
    CCMenuItemToggler* m_bulkSelectToggler;

};
