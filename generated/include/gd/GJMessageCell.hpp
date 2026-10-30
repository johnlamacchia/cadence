#pragma once

class CCMenuItemToggler;
class FLAlertLayer;
class GJUserMessage;
class UploadActionPopup;
class cocos2d;

class GJMessageCell : public TableViewCell, public FLAlertLayerProtocol, public UploadPopupDelegate, public UploadActionDelegate {
public:
    virtual bool init();
    virtual void draw();
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void onClosePopup(UploadActionPopup* popup);

    GJMessageCell(char const* identifier, float width, float height);
    ~GJMessageCell();
    void loadFromMessage(GJUserMessage* message);
    void markAsRead();
    void onDeleteMessage(cocos2d::CCObject* sender);
    void onToggle(cocos2d::CCObject* sender);
    void onViewMessage(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void updateBGColor(int index);
    void updateToggle();

    GJUserMessage* m_message;
    UploadActionPopup* m_popup;
    CCMenuItemToggler* m_toggler;

};
