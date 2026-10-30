#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class FLAlertLayer;
class GJLevelList;
class SelectListIconLayer;
class SetIDPopup;
class ShareCommentLayer;
class TableView;
class TableViewCell;
class cocos2d;

class LevelListLayer : public LevelBrowserLayer, public TextInputDelegate, public SelectListIconDelegate, public LikeItemDelegate, public LevelListDeleteDelegate, public AdminActionConfirmDelegate {
public:
    virtual void onEnter();
    virtual void onExit();
    virtual void loadLevelsFinished(cocos2d::CCArray* levels, char const* key, int type);
    virtual void loadLevelsFailed(char const* key, int type);
    virtual void onBack(cocos2d::CCObject* sender);
    virtual void shareCommentClosed(std::string text, ShareCommentLayer* layer);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void setIDPopupClosed(SetIDPopup* popup, int value);
    virtual cocos2d::CCArray* updateResultArray(cocos2d::CCArray* results);
    virtual bool cellPerformedAction(TableViewCell* cell, int listType, CellAction action, cocos2d::CCNode* parent);
    virtual void likedItem(LikeItemType type, int id, bool liked);
    virtual void iconSelectClosed(SelectListIconLayer* layer);
    virtual void levelListDeleteFinished(int id);
    virtual void levelListDeleteFailed(int id);
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void adminActionConfirmed(int, std::vector<int> const&, std::string const&);

    static LevelListLayer* create(GJLevelList* list);
    static cocos2d::CCScene* scene(GJLevelList* list);

    LevelListLayer();
    ~LevelListLayer();
    void cloneList();
    void confirmClone(cocos2d::CCObject* sender);
    void confirmDelete(cocos2d::CCObject* sender);
    void confirmOwnerDelete(cocos2d::CCObject* sender);
    bool init(GJLevelList* list);
    void onClaimReward(cocos2d::CCObject* sender);
    void onDelete();
    void onDescription(cocos2d::CCObject* sender);
    void onFavorite(cocos2d::CCObject* sender);
    void onInfo(cocos2d::CCObject* sender);
    void onLike(cocos2d::CCObject* sender);
    void onListInfo(cocos2d::CCObject* sender);
    void onRateListAdmin(cocos2d::CCObject* sender);
    void onRefreshLevelList(cocos2d::CCObject* sender);
    void onSelectIcon(cocos2d::CCObject* sender);
    void onShare(cocos2d::CCObject* sender);
    void onToggleEditMode(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void ownerDelete();
    void /* TodoReturn */ startServerDelete(std::string const&, std::vector<int> const&);
    void updateEditMode();
    void updateSideButtons();
    void updateStatsArt();
    void verifyListName();

    cocos2d::CCMenu* m_buttonMenu;
    std::string m_searchKey1;
    GJLevelList* m_levelList;
    std::string m_searchKey2;
    CCMenuItemSpriteExtra* m_likeButton;
    bool m_exiting;
    CCTextInputNode* m_titleInput;
    int m_editMode;
    cocos2d::CCArray* m_objects;
    cocos2d::CCSprite* m_diffSprite;
    cocos2d::CCSprite* m_featureSprite;
    cocos2d::CCPoint m_rewardPosition;
    bool m_exited;

};
