#pragma once

class GJComment;
class cocos2d;

class CommentOptionsPopup : public SetupTriggerPopup {
public:
    virtual ~CommentOptionsPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);

    static CommentOptionsPopup* create(GJComment*, bool);

    bool init(GJComment*, bool);
    void onCopy(cocos2d::CCObject* sender);
    void onReport(cocos2d::CCObject* sender);

};
