#pragma once

class DialogLayer;
class cocos2d;

class LevelAreaLayer : public cocos2d::CCLayer, public DialogDelegate {
public:
    virtual bool init();
    virtual void keyBackClicked();
    virtual void dialogClosed(DialogLayer* layer);
    virtual void onExit();

    static void addTorch(cocos2d::CCNode* parent, cocos2d::CCPoint position, int fireType, float scale, int zOrder, bool noTorch, int colorType, cocos2d::CCArray* nodes);
    static LevelAreaLayer* create();
    static cocos2d::CCScene* scene();

    LevelAreaLayer();
    ~LevelAreaLayer();
    void addGodRay(float startOpacity, float midOpacity, float endOpacity, float fadeDuration, float scaleY, cocos2d::CCPoint position);
    void fadeInsideTower();
    void onBack(cocos2d::CCObject* sender);
    void onClickDoor(cocos2d::CCObject* sender);
    bool onEnterTower();
    void showDialog();

    cocos2d::CCSprite* m_towerSprite;
    cocos2d::CCArray* m_godRays;
    bool m_enteringTower;
    bool m_exiting;

};
