#pragma once

class CCMenuItemSpriteExtra;
class GJLevelList;
class LoadingCircle;
class TextArea;
class cocos2d;

class UploadListPopup : public FLAlertLayer, public ListUploadDelegate {
public:
    virtual void keyBackClicked();
    virtual void show();
    virtual void listUploadFinished(GJLevelList* list);
    virtual void listUploadFailed(GJLevelList* list, int response);

    static UploadListPopup* create(GJLevelList* list);

    UploadListPopup();
    ~UploadListPopup();
    bool init(GJLevelList* list);
    void onBack(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);
    void onReturnToList(cocos2d::CCObject* sender);

    GJLevelList* m_levelList;
    TextArea* m_textArea;
    LoadingCircle* m_loadingCircle;
    CCMenuItemSpriteExtra* m_backButton;

};
