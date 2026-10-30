#pragma once

class CCTextInputNode;
class FLAlertLayer;
class GJComment;
class GJCommentListLayer;
class UploadActionPopup;
class cocos2d;

class MPLobbyLayer : public cocos2d::CCLayer, public GJMPDelegate, public UploadPopupDelegate, public UploadActionDelegate, public FLAlertLayerProtocol, public TextInputDelegate {
public:
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void joinLobbyFinished(int id);
    virtual void joinLobbyFailed(int id, GJMPErrorCode errorType);
    virtual void updateComments();
    virtual void didUploadMPComment(int id);
    virtual void textInputOpened(CCTextInputNode* node);
    virtual void textInputClosed(CCTextInputNode* node);
    virtual void textChanged(CCTextInputNode* node);
    virtual void keyUp(cocos2d::enumKeyCodes key, double timestamp);
    virtual void FLAlert_Clicked(FLAlertLayer* layer, bool btn2);
    virtual void uploadActionFinished(int id, int response);
    virtual void uploadActionFailed(int id, int response);
    virtual void onClosePopup(UploadActionPopup* popup);

    static MPLobbyLayer* create(int type);
    static cocos2d::CCScene* scene(int type);

    bool init(int type);
    void onBack(cocos2d::CCObject* sender);
    void onBtn1(cocos2d::CCObject* sender);
    void onBtn2(cocos2d::CCObject* sender);
    void onBtn3(cocos2d::CCObject* sender);
    void onComment(cocos2d::CCObject* sender);
    void onUpdateLobby();
    void postComment();
    void tryExitLobby();
    void updateLobby(float delay);

    int m_lobbyID;
    cocos2d::CCLabelBMFont* m_lobbyLabel;
    UploadActionPopup* m_uploadPopup;
    GJCommentListLayer* m_listLayer;
    CCTextInputNode* m_commentInput;
    float m_lobbyDelay;
    bool m_awaitingLobby;
    bool m_lobbyJoined;

};
