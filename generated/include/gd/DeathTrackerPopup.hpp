#pragma once

class DTStatSeg;
class GJGameLevel;
class SetIDPopup;
class cocos2d;

class DeathTrackerPopup : public FLAlertLayer, public VirtualRowSource, public SetIDPopupDelegate {
public:
    virtual ~DeathTrackerPopup();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyBackClicked();
    virtual void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    virtual void show();
    virtual void setIDPopupClosed(SetIDPopup*, int);
    virtual int virtualRowCount();
    virtual int virtualRowHeight(int);
    virtual void mountVirtualRow(int, cocos2d::CCNode*, cocos2d::CCMenu*);

    static DeathTrackerPopup* create(GJGameLevel*);

    void /* TodoReturn */ addRow(cocos2d::CCNode*, std::string const&, float, cocos2d::ccColor3B, float);
    void /* TodoReturn */ buildAttemptLine();
    void /* TodoReturn */ buildHistory();
    void /* TodoReturn */ buildList();
    void /* TodoReturn */ buildStatRow(std::vector<DTStatSeg> const&, cocos2d::CCNode*);
    void /* TodoReturn */ countsForScope(std::vector<int>&);
    void /* TodoReturn */ dtScrollKey();
    bool init(GJGameLevel*);
    void onClose(cocos2d::CCObject* sender);
    void onJumpCurrent(cocos2d::CCObject* sender);
    void onJumpSession(cocos2d::CCObject* sender);
    void onJumpSummary(cocos2d::CCObject* sender);
    void onNextSession(cocos2d::CCObject* sender);
    void onOptions(cocos2d::CCObject* sender);
    void onPanelButton(cocos2d::CCObject* sender);
    void onPrevSession(cocos2d::CCObject* sender);
    void onToggleGlobal(cocos2d::CCObject* sender);
    void /* TodoReturn */ parseGlobal();
    void /* TodoReturn */ parseSessions();
    void /* TodoReturn */ refreshGlobal();
    void /* TodoReturn */ refreshView();
    void setView(int);
    void /* TodoReturn */ updatePanel();

};
