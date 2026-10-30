#pragma once

class CCMenuItemSpriteExtra;
class CCTextInputNode;
class EmotePickerPopup;
class FLAlertLayer;
class GJGameLevel;
class SetIDPopup;
class UploadActionPopup;
class cocos2d;

class EditLevelLayer : public cocos2d::CCLayer, public TextInputDelegate, public FLAlertLayerProtocol, public UploadActionDelegate, public UploadPopupDelegate, public SetIDPopupDelegate, public CreatorSettingsDelegate, public EmotePickerDelegate {
public:
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void setIDPopupClosed(SetIDPopup* popup, int value);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void onClosePopup(UploadActionPopup* popup);
    virtual void creatorDescriptionChanged(std::string const&);
    virtual void emotePickerClosed(EmotePickerPopup*);

    static EditLevelLayer* create(GJGameLevel* level);
    static cocos2d::CCScene* scene(GJGameLevel* level);

    EditLevelLayer();
    ~EditLevelLayer();
    void closeTextInputs();
    void confirmClone(cocos2d::CCObject* sender);
    void confirmDelete(cocos2d::CCObject* sender);
    void /* TodoReturn */ confirmMoveToBottom(cocos2d::CCObject*);
    void confirmMoveToTop(cocos2d::CCObject* sender);
    bool init(GJGameLevel* level);
    void onBack(cocos2d::CCObject* sender);
    void onClone();
    void onCreatorSettings(cocos2d::CCObject* sender);
    void onDeathTracker(cocos2d::CCObject* sender);
    void onDelete();
    void onEdit(cocos2d::CCObject* sender);
    void onEmotes(cocos2d::CCObject* sender);
    void onExportLevel(cocos2d::CCObject* sender);
    void onLevelInfo(cocos2d::CCObject* sender);
    void onLevelLeaderboard(cocos2d::CCObject* sender);
    void onLevelOptions(cocos2d::CCObject* sender);
    void /* TodoReturn */ onMoveToBottom();
    void onMoveToTop();
    void onPlay(cocos2d::CCObject* sender);
    void onSetFolder(cocos2d::CCObject* sender);
    void onShare(cocos2d::CCObject* sender);
    void onTest(cocos2d::CCObject* sender);
    void onUpdateDescription(cocos2d::CCObject* sender);
    void playStep2();
    void playStep3();
    void setupLevelInfo();
    void updateDescText(char const* text);
    void verifyLevelName();

    cocos2d::CCMenu* m_buttonMenu;
    GJGameLevel* m_level;
    cocos2d::CCArray* m_textInputs;
    cocos2d::CCLabelBMFont* m_folderLabel;
    bool m_exiting;
    GJLevelType m_levelType;
    std::string m_levelName;
    UploadActionPopup* m_descriptionPopup;
    bool m_loadPlatformerSave;
    CCMenuItemSpriteExtra* m_emoteButton;
    EmotePickerPopup* m_emotePicker;
    std::string m_levelDesc;

};
