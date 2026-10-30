#pragma once

class GJBigSprite;
class GJMapObject;
class LevelEditorLayer;
class SimplePlayer;
class cocos2d;

class MapSelectLayer : public cocos2d::CCLayer {
public:
    virtual void update(float dt);
    virtual bool init();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void registerWithTouchDispatcher();
    virtual void keyBackClicked();
    virtual bool scrollWheel(float y, float x);

    static MapSelectLayer* create();
    static cocos2d::CCScene* scene();

    bool checkTouchMonster(cocos2d::CCPoint position);
    GJMapObject* createObjectAtPoint(cocos2d::CCPoint position);
    cocos2d::CCPoint getConstrainedMapPos(cocos2d::CCPoint position);
    void onBack(cocos2d::CCObject* sender);

    GJBigSprite* m_bigSprite;
    SimplePlayer* m_player;
    cocos2d::CCNode* m_mainLayer;
    bool m_unloadTexturesOnExit;
    bool m_backPressed;
    bool m_blockExit;
    bool m_inTouch;
    double m_lastTouchTime;
    bool m_editMode;
    cocos2d::CCPoint m_lastTouchPos;
    cocos2d::CCPoint m_mapPos;
    cocos2d::CCArray* m_monsters;
    LevelEditorLayer* m_editorLayer;

};
