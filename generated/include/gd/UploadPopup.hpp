#pragma once

class CCMenuItemSpriteExtra;
class GJGameLevel;
class LoadingCircle;
class TextArea;
class cocos2d;

class UploadPopup : public FLAlertLayer, public LevelUploadDelegate {
public:
    virtual void keyBackClicked();
    virtual void show();
    virtual void levelUploadFinished(GJGameLevel* level);
    virtual void levelUploadFailed(GJGameLevel* level);

    static UploadPopup* create(GJGameLevel* level);

    UploadPopup();
    ~UploadPopup();
    bool init(GJGameLevel* level);
    void onBack(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onReturnToLevel(cocos2d::CCObject* sender);

    GJGameLevel* m_level;
    TextArea* m_textArea;
    LoadingCircle* m_loadingCircle;
    CCMenuItemSpriteExtra* m_backButton;

};
