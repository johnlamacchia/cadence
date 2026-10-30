#pragma once

class cocos2d;

class LevelBadgePopup : public FLAlertLayer, public LevelManagerDelegate {
public:
    virtual ~LevelBadgePopup();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void keyBackClicked();
    virtual void show();
    virtual void loadLevelsFinished(cocos2d::CCArray*, char const*, int);
    virtual void loadLevelsFailed(char const*, int);

    static LevelBadgePopup* create(int, bool);

    void /* TodoReturn */ buildCell(cocos2d::CCObject*);
    bool init(int, bool);
    void onClose(cocos2d::CCObject* sender);

};
