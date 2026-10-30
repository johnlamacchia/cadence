#pragma once

class CCMenuItemToggler;
class GJMessageThread;
class cocos2d;

class GJThreadCell : public TableViewCell {
public:
    virtual ~GJThreadCell();
    virtual bool init();
    virtual void draw();

    GJThreadCell(char const*, float, float);
    void loadFromThread(GJMessageThread*);
    void onShowMembers(cocos2d::CCObject* sender);
    void onThreadOptions(cocos2d::CCObject* sender);
    void onToggleSelect(cocos2d::CCObject* sender);
    void onViewProfile(cocos2d::CCObject* sender);
    void onViewThread(cocos2d::CCObject* sender);
    void updateBGColor(int);

    GJMessageThread* m_thread;
    CCMenuItemToggler* m_toggler;

};
