#pragma once

class CCTextInputNode;
class ThreadViewPopup;
class cocos2d;

class ThreadSearchPopup : public SetupTriggerPopup {
public:
    virtual ~ThreadSearchPopup();
    virtual void keyBackClicked();
    virtual void onClose(cocos2d::CCObject* sender);
    virtual void textChanged(CCTextInputNode*);

    static ThreadSearchPopup* create(ThreadViewPopup*);

    void /* TodoReturn */ clampDayField();
    void /* TodoReturn */ fieldValue(CCTextInputNode*, int, int);
    bool init(ThreadViewPopup*);
    void onGoToDate(cocos2d::CCObject* sender);
    void onSearch(cocos2d::CCObject* sender);

};
