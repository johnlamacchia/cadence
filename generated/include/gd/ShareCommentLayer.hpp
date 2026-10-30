#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class EmotePickerPopup;
class ShareCommentDelegate;
class UploadActionPopup;
class cocos2d;

class ShareCommentLayer : public FLAlertLayer, public UploadActionDelegate, public UploadPopupDelegate, public EmotePickerDelegate {
public:
    virtual ~ShareCommentLayer();
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void onClosePopup(UploadActionPopup* popup);
    virtual void emotePickerClosed(EmotePickerPopup*);

    static ShareCommentLayer* create(std::string title, int charLimit, CommentType type, int ID, std::string desc);

    bool init(std::string title, int charLimit, CommentType type, int ID, std::string desc);
    void /* TodoReturn */ lockInput(std::string const&);
    void onClear(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onEmotes(cocos2d::CCObject* sender);
    void onPercent(cocos2d::CCObject* sender);
    void onShare(cocos2d::CCObject* sender);
    void updateCharCountLabel();
    void updateDescText(std::string desc);
    void updatePercentLabel();

    int m_charLimit;
    int m_itemID;
    CommentType m_commentType;
    bool m_allowEmotes;
    CCTextInputNode* m_commentInput;
    CCMenuItemSpriteExtra* m_emotesButton;
    EmotePickerPopup* m_emotePicker;
    std::string m_descText;
    std::string m_placeholderText;
    cocos2d::CCLabelBMFont* m_charCountLabel;
    cocos2d::CCLabelBMFont* m_percentLabel;
    UploadActionPopup* m_uploadPopup;
    bool m_uploadSuccess;
    bool m_percentEnabled;
    int m_percent;
    ShareCommentDelegate* m_delegate;

};
