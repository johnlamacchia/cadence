#pragma once

class CCMenuItemSpriteExtra;
class RateLevelDelegate;
class UploadActionPopup;
class cocos2d;

class RateDemonLayer : public FLAlertLayer, public UploadPopupDelegate, public UploadActionDelegate {
public:
    virtual void keyBackClicked();
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void onClosePopup(UploadActionPopup* popup);

    static RateDemonLayer* create(int levelID);

    RateDemonLayer();
    ~RateDemonLayer();
    bool init(int levelID);
    void onClose(cocos2d::CCObject* sender);
    void onRate(cocos2d::CCObject* sender);
    void selectRating(cocos2d::CCObject* sender);

    bool m_uploadFinished;
    CCMenuItemSpriteExtra* m_submitButton;
    cocos2d::CCArray* m_demons;
    void* m_unkPtr;
    int m_levelID;
    int m_demonRate;
    bool m_moderator;
    UploadActionPopup* m_popup;
    RateLevelDelegate* m_delegate;

};
